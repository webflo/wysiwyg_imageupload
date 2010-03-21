<?php
// $Id: wysiwyg_imageupload.ajax.inc,v 1.1.2.20 2010/03/19 11:20:43 eugenmayer Exp $

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer

/*
 * Called through ajax(AHAH), uploading an image and replacing
 * the upload form with an image-details form, if successfully uploaded
 * Some of the code is shared from the upload.module
 */
function _wysiwyg_imageupload_upload_js($node_form_build_id) {
  // Load the form from the Form API cache.
  $cached_form_state = array();
  $form_state = array();

  $cached_form = form_get_cache($node_form_build_id, $cached_form_state);
  dsm($node_form_build_id);
  dsm($cached_form);
  if (!$cached_form) {
    // no cached form for e.g. comment, use custom cache using cache_set/get
    $cached_form = array();
    $cached_form['#no_cache'] = TRUE;
    // #node is our transport item for upload file data - in any case
    $cached_form['#node'] = new stdClass();
  }

  // Handle new uploads, and merge tmp files into node-files.
  $imgDetails = _wysiwyg_imageupload_upload_file($cached_form,$form_state);

  // remove all status messages
  //drupal_get_messages('status',true);
  if($imgDetails->filepath != "") {
    dsm($cached_form);
    if ($cached_form['#no_cache'] === TRUE) {
      $cache_id = 'wysiwyg_imageupload_'.$node_form_build_id;
      dsm($cache_id);
      cache_set($cache_id, $cached_form['#node']->imgupload_images, 'cache', CACHE_TEMPORARY);
      dsm(cache_get($cache_id));
    }
    else {
      form_set_cache($node_form_build_id, $cached_form, $cached_form_state);
    }

    // We save the fielpath in our cache, so we dont need to pass it later as a GET variable.
    // (when we let the html object generate using ajax).
    $cacheid = uniqid();
    cache_set($cacheid,$imgDetails->filepath,'cache',CACHE_TEMPORARY);

    // Save the main form in the cache, adding the file.
    $output = theme('status_messages');
    // No errors, so lets replace the upload widget with the image details
    $title = $imgDetails->filename;

    // This is our thumbnail. It also saves the cacheid in alt. We will inject this into the details form.
    // TODO: maybe really generate a small thumbnail and not reuse / resize the original using the browser,
    $img = "<img id='uploadedImage' title='$title' alt='$cacheid' height='200px;border:1px solid black;margin:auto' src='". base_path() . $imgDetails->filepath ."'>".$output;

    // Create the form image detail form.
    $form = array();
    $form_state = array();
    $form = _wysiwyg_imageupload_details_form($title,$img);
    $form = form_builder('wysiwyg_imageupload_detailsform', $form, $form_state);
    $output = drupal_render($form).$output;
  }
  else {
    drupal_set_message(t('Select a file to upload first'),'error');
    $output = theme('status_messages');

    $form_state = array();
    $form = array();
    $form['image_upload'] = array( '#type' => 'fieldset',
      '#title'=> t('Upload image'),
      '#prefix' => '<div id="file_upload_group">',
      '#suffix' => '</div>');
    _wysiwyg_imageupload_form_add_uploadfields($form,$form_id);
    $form = form_builder('wysiwyg_imageupload_upload', $form, $form_state);
    $output .= $output.drupal_render($form);
  }
  // As we are an AHAH callback, lets convert to JSON.
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}

/*
 * Ajax callback.
 * Using the cacheID to determine the filepath and returns a html image object
 * If imagecache is installed, use the preset
 */
function _wysiwyg_imageupload_show_image($cacheID,$preset,$title) {
   $preset = check_plain($preset);
   $title = check_plain($title);

   if (module_exists('imagecache')) {
    $filepath = cache_get($cacheID,'cache');
    $GLOBALS['devel_shutdown'] = FALSE;
    $output = theme('imagecache',$preset,$filepath->data, $filepath->data,$title);
    print $output;
    exit;
   }
}
