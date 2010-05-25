<?php
// $Id:$

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer

/*
 * Called through ajax(AHAH), uploading an image and replacing
 * the upload form with an image-details form, if successfully uploaded.
 */
function _wysiwyg_imageupload_entity_upload_js($node_form_build_id) {
  // Load the cache which includes the currently allready uploaded files.
  $cached_files = cache_get('wysiwyg_imageupload_'.$node_form_build_id);

  // Yet no file, initialize the array.
  if(empty($cached_files)) {
    $cached_files = new stdClass();
    $cached_files->data = array();
  }
  $cached_files = $cached_files->data;

  // Handle new uploads. Create the files in the files table and make them temperory.
  // The will need to be made persistend on form submit in the hook_comment or hook_nodeapi
  $imgDetails = _wysiwyg_imageupload_upload_file();

  if ($imgDetails->filepath != "") {
    // save the fid as used inline and generate a iid (inline id)
    $iid = _wysiwyg_imageupload_craete_inline_entity($imgDetails,true);
    // we store the iid in the file object, so we can use it on submit of the node form
    // to update the inline-entry to be static and not temporary
    $imgDetails->iid = $iid;
    $cached_files[$iid] = $imgDetails;
    cache_set( 'wysiwyg_imageupload_'.$node_form_build_id, $cached_files, 'cache', CACHE_TEMPORARY );

    $title = $imgDetails->filename;
    $attributes = array(
      'class' => 'wysiwyg_imageupload_preview',
    );

    $img = theme('imagecache', 'wysiwyg_imageupload_preview', $imgDetails->filepath, $iid, $title, $attributes, TRUE, FALSE);

    // load the old upload form, so we can extend it with the new fields
    $cached_form_state = array();
    $form = form_get_cache($_POST['form_build_id'], $cached_form_state);

    // add our details form.
    $details = _wysiwyg_imageupload_details_form($img,$iid,array('title' => $title));
    $form += $details;
    form_set_cache($_POST['form_build_id'], $form, $cached_form_state );

    // Render the new partial form for output.
    $form_state = array('submitted' => FALSE);
    $form = array(
      '#post' => $_POST,
      '#programmed' => FALSE,
      '#tree' => FALSE,
      '#parents' => array(),
    );

    $form += $details;
    $form = form_builder('wysiwyg_imageupload_details_form', $form, $form_state);
    $output = theme('status_messages') . drupal_render($form);
  }
  else {
    drupal_set_message(t('Invalid image (size?). Please select a proper image.'),'error');
    $output = theme('status_messages','error');

    $form_state = array();
    $form = array();
    _wysiwyg_imageupload_form_add_uploadfields($form,$node_form_build_id);
    $form = form_builder('wysiwyg_imageupload_upload', $form, $form_state);
    $output = $output . drupal_render($form);
  }
  // As this is an AHAH callback, lets convert to JSON and return.
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}

/**
 * Ajax callback to render a image for the wysiwyg editor
 */
function _wysiwyg_imageupload_render_wysiwyg($iid) {
  $image_obj = _wysiwyg_imageupload_load_inline_entity($iid);
  $output = theme('wysiwyg_imageupload_render_image_wysiwyg', $image_obj, array());
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit();
}

/**
 * Ajax callback to render some images for the wysiwyg editor
 */
function _wysiwyg_imageupload_render_wysiwyg_images($iids, $revisioned, $form_id) {
  $iids = split(',',$iids);
  $output = array();
  foreach($iids as $iid) {
    $image_obj = _wysiwyg_imageupload_load_inline_entity($iid);
    $output[$iid] = theme('wysiwyg_imageupload_render_image_wysiwyg', $image_obj, array());
  }
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit();
}

/**
 * Helper to convert a inline-image into a backend tag
 */
function _wysiwyg_imageupload_render_backend($iid,$arguments = "") {
  $image_obj = _wysiwyg_imageupload_load_inline_entity($iid);
  return "[[wysiwyg_imageupload:$iid:$arguments]]";
}