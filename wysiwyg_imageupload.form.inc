<?php
// $Id:$

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer



/*
 * Implementation of hook_form_alter
 * Adding jquery_ui_dialog javascript / css definitions to the form.
 */
function wysiwyg_imageupload_form_alter(&$form, $form_state, $form_id){
  if ($form['#id'] == 'node-form' || $form['#id'] == 'comment-form' ) {
    // We need $form_state['post']['form_build_id'] != "" as a additional condition, as there are cases that
    // post is an empty array but it has not been submitted ( no preview, no save).
    if(isset($form_state['post']) && is_array($form_state['post']) && $form_state['post']['form_build_id'] != '') {
      // The form has been saved or is preview, transfer the cache to the new form_build_id
      _wysiwyg_imageupload_transfer_cache($form, $form_state);
    }
    else {
      // Load already existing files and put them in the array before the form is shown.
      // This must be done when nodes or comments are edited, to ensure that already uploaded images
      // are in the cache.
      if (
        $form['#id'] == 'comment-form' &&   // Should only match, if we have a comment formular.
        $form['cid']['#value'] != NULL      // If its a creation of a new comment, we dont need to load any files.
      ) {
        // We need this special case, as there is no "op=load" in the hook_comment hook_comment
        // so we cant load extra data like our files. We only call this on the first load of the edit form
        // so on a preview or similar, we already have written this cache and only load (transfer) it.
        $files = _wysiwyg_imageupload_commment_load($form['cid']['#value']);
      }
      else if (
        $form['#id'] == 'node-form' &&
        $form['#node']->nid != NULL &&
        !empty($form['#node']->imgupload_images)
      ) {
        $files = $form['#node']->imgupload_images;
      }
      // Save them in the cache, if there are any.
      if(is_array($files) && count($files) > 0) {
        cache_set('wysiwyg_imageupload_'.$form['#build_id'],$files,'cache',CACHE_TEMPORARY);
      }
    }
    // Comment form is incompatible with form caching see http://drupal.org/node/644222
    if($form['#id'] != 'comment-form') {
      // Thats is not mandatory anymore, as we handle or cache ourselfs
      $form['#cache'] = TRUE;
    }

    drupal_add_js(array('wysiwyg_imageupload' => array('current_form' => $form['form_build_id']['#value'])), 'setting');
    jquery_ui_dialog_parent_js();

    $form['#validate'][] = 'wysiwyg_imageupload_form_validate';
  }
}

/*
 * This method transfers the cache of the old from_build_id to a new cache setting
 * using the new form_build_id. This is need due every preview and save of a non cached form
 * is causing the form_build_id to change and therefor we could later not load our uploaded files
 * for making them persisten.
 */
function _wysiwyg_imageupload_transfer_cache($form, $form_state) {
  $old_form_build_id = $form_state['post']['form_build_id'];
  $new_form_build_id = $form['#build_id'];
  // Lets get the old cache and set it to the new cache
  $old = cache_get('wysiwyg_imageupload_'.$old_form_build_id);
  // Now apply it using the new from_build_id
  cache_set('wysiwyg_imageupload_'.$new_form_build_id,$old->data,'cache',CACHE_TEMPORARY);
}

/*
 * Called on every node or comment save
 * This method will load the cache and put the loaded files into the comment-object for use in insert/update
 * where those files will be made persistent and added to the relation table.
 */
function wysiwyg_imageupload_form_validate(&$form, &$form_state) {
  if ($form_state['submitted'] == true) {
    $value = cache_get('wysiwyg_imageupload_' . $form_state['values']['form_build_id'], 'cache');
    $form_state['values']['imgupload_images'] = $value->data;
  }
}

/*
 * Returns the general image upload form.
 * This form is shown before an image has been uploaded,
 * afterwards it is replaced by the image details form
 */
function wysiwyg_imageupload_upload_form(&$form_state,$form_id) {
  $form_id = check_plain($form_id);
  jquery_ui_dialog_child_js();
  module_invoke('admin_menu', 'suppress');
  drupal_set_title(t('Upload image'));
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['image_upload'] = array(
    '#type' => 'fieldset',
    '#title'=> t('Upload image'),
    '#prefix' => '<div id="file_upload_group">',
    '#suffix' => '</div>');
  _wysiwyg_imageupload_form_add_uploadfields($form,$form_id);

  return $form;
}

/*
 * Return the form to be presented as image details form.
 * Title, floating description and so forth
 */
function _wysiwyg_imageupload_details_form($title,$img,$defaults = array()){
  // TODO: would love to add some drupal_alter stuff here, but we will need to
  // have something similar on the javascript side.
  $form = array();
  $form['image_upload'] = array( '#type' => 'fieldset',
    '#title'=> t('Upload picture'),
    '#prefix' => '<div id="file_upload_group">',
    '#suffix' => '</div>'
  );
  $form['image_upload']['uploadedImage'] = array (
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#value' => $img
  );
  $form['image_upload']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $title,
    '#size' => 50
  );

  $form['image_upload']['alignment'] = array(
    '#type' => 'select',
    '#title' => t('Text floating'),
    '#options' => array(
      'imgupl_floating_none' => t('Below image (none)'),
      'imgupl_floating_left' => t('Text right beside image'),
      'imgupl_floating_right' => t('Text left beside image')
    ),
    '#default_value' => $defaults['floating']
  );

  $form['image_upload']['preset'] = array(
    '#type' => 'select',
    '#title' => t('Size'),
    '#default_value' => $defaults['preset'],
    '#options' => _wysiwyg_imagegupload_allowed_presets(),
  );

  // styles.
  $styles = _wysiwyg_imageupload_get_image_styles();
  if(count($styles) > 0) { // If any.
    array_unshift($styles, t('None'));
    $form['image_upload']['style'] = array(
      '#type' => 'select',
      '#title' => t('Style'),
      '#options' => $styles,
      '#default_value' => $defaults['style'],
    );
  }

  return $form;
}

/*
 * Returning the form elements for the image upload
 */
function _wysiwyg_imageupload_form_add_uploadfields(&$form, $form_id) {
  $form['image_upload']['wysiwyg_imageupload_file'] = array(
    '#type' => 'file',
    '#title' => t('Choose an image'),
    '#size' => 40,
  );
  $form['image_upload']['start_upload'] = array(
    '#type' => 'submit',
    '#value' => t('Upload image'),
    '#name' => 'attach',
    '#ahah' => array(
      'path' => "ajax/wysiwyg_imgupl/upload/$form_id",
      'wrapper' => 'file_upload_group',
      'progress' => array('type' => 'bar', 'message' => t('Please wait...'))
    )
  );
}

/*
 * Called when a image was selected and the details form should be shown.
 */
function wysiwyg_imageupload_update_form(&$form_state, $title, $preset, $floating, $style) {
  // security
  $title = check_plain($title);
  $imgpath = check_url(urldecode($_GET['imagepath']));

  jquery_ui_dialog_child_js();
  module_invoke('admin_menu', 'suppress');
  drupal_set_title(t('Update image'));

  if(!file_exists(realpath($imgpath))) {
    drupal_set_message("The image does not exist ($imgpath)",'error');
    return array();
  }

  // Save the image path in the cache to get it back later
  $cacheid = uniqid();
  cache_set($cacheid,$_GET['imagepath'],'cache',CACHE_TEMPORARY);

  $img = "<img id='uploadedImage' title=\"$title\" alt='$cacheid' height='200px;border:1px solid black;margin:auto' src='".base_path().$imgpath."'>";

  $defaults = array (
    'filepath' => $imgpath,
    'preset' => check_plain(urldecode($preset)),
    'floating' => check_plain(urldecode($floating)),
    'style' =>check_plain(urldecode($style)),
  );

  // Create the form image detail form.
  $form = _wysiwyg_imageupload_details_form($title,$img,$defaults);

  return $form;
}
