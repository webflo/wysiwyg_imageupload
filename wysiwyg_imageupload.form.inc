<?php
// $Id$

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer

/*
 * Called on every node save
 */
function wysiwyg_imageupload_form_submit(&$form, &$form_state) {
  // We need to do those that, otherwise our saved images in the cache
  // are not included in the node object when the nodeapi is called
  if(!empty($form['#node']->images) && is_array($form['#node']->images)) {
    $form_state['values']['images'] = $form['#node']->images;
  }
}

/*
 * Returns the general image upload form.
 * This form is shown before an image has been uploaded,
 * afterwards it is replaced by the image details form
 */
function wysiwyg_imageupload_upload_form(&$form_state,$form_id) {
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
    '#default_value' => $defaults['floating']);

   // ImageCache support.
  if (module_exists('imagecache')) {
    $presets = imagecache_presets();
    $styles = array();
    foreach ($presets as $preset) {
      // We can use the presetid here (http://drupal.org/node/694188)
      $styles[$preset['presetname']] = t($preset['presetname']);
    }
    $form['image_upload']['preset'] = array(
      '#type' => 'select',
      '#title' => t('Size'),
      '#options' => $styles,
      '#default_value' => $defaults['preset']
    );
  }

  // styles.
  $styles = _wysiwyg_imageupload_get_image_styles();
  if(count($styles) > 0) { // If any.
    array_unshift($styles, t('None'));
    $form['image_upload']['style'] = array(
      '#type' => 'select',
      '#title' => t('Style'),
      '#options' => $styles,
      '#default_value' => $defaults['style']
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
  jquery_ui_dialog_child_js();
  module_invoke('admin_menu', 'suppress');
  drupal_set_title(t('Update image'));
  $cacheid = uniqid();
  cache_set($cacheid,$_GET['imagepath'],'cache',CACHE_TEMPORARY);
  $img = "<img id='uploadedImage' title=\"$title\" alt='$cacheid' height='200px;border:1px solid black;margin:auto' src='". base_path() . $_GET['imagepath']."'>";

  $defaults = array (
    'filepath' => check_plain(urldecode($_GET['imagepath'])),
    'preset' => check_plain(urldecode($preset)),
    'floating' => check_plain(urldecode($floating)),
    'style' =>check_plain(urldecode($style)),
  );

  // Create the form image detail form.
  $form = _wysiwyg_imageupload_details_form($title,$img,$defaults);

  return $form;
}
