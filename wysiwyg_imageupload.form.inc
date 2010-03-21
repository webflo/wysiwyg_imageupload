<?php
// $Id: wysiwyg_imageupload.form.inc,v 1.1.2.20 2010/03/19 11:20:43 eugenmayer Exp $

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer



/*
 * Implementation of hook_form_alter
 * Adding jquery_ui_dialog javascript / css definitions to the form
 */
function wysiwyg_imageupload_form_alter(&$form, $form_state, $form_id){
  // Even if the wysiwyg_imageupload button is not activate, we need to load the
  // jquery_ui_dialog preconditions. The user might switch the editor on the node form
  // then we would need it

  // Check, if thats a Preview. Because every preview generates a new form_build_id


  if ($form['#id'] == 'node-form' || $form['#id'] == 'comment-form' ) {
    // We need $form_state['post']['form_build_id'] != "" as a additional condition, as there are cases that
    // post is an empty array but it has not been submitted ( no preview, no save).
    if(isset($form_state['post']) && is_array($form_state['post']) && $form_state['post']['form_build_id'] != '') {
      dsm($form_state['post']);
      dsm('transfer cache');
      _wysiwyg_imageupload_transfer_cache($form, $form_state);
    }
    // Comment form is incompatible with form caching see
    // http://drupal.org/node/644222
    if($form['#id'] != 'comment-form') {
      // Activate form caching is we will need it by AHAH later
      // case: node form + node preview submit
      dsm('activating cache');
      $form['#cache'] = TRUE;
    }

    drupal_add_js(array('wysiwyg_imageupload' => array('current_form' => $form['form_build_id']['#value'])), 'setting');
    jquery_ui_dialog_parent_js();
    $form['#submit'][] = 'wysiwyg_imageupload_form_submit';
  }
}

function _wysiwyg_imageupload_transfer_cache($form, $form_state) {
  $old_form_build_id = $form_state['post']['form_build_id'];
  $new_form_build_id = $form['#build_id'];
  dsm($old_form_build_id);
  dsm($new_form_build_id);
  // Lets get the old cache and set it to the new cache
  $old = cache_get('wysiwyg_imageupload_'.$old_form_build_id);
  // Now apply it using the new from_build_id
  dsm($old->data);
  cache_set('wysiwyg_imageupload_'.$new_form_build_id,$old->data,'cache',CACHE_TEMPORARY);
  $d = cache_get('wysiwyg_imageupload_'.$new_form_build_id);
  dsm('new data:');
  dsm($d->data);
}

/*
 * Called on every node save
 */
function wysiwyg_imageupload_form_submit(&$form, &$form_state) {
  // We need to do those that, otherwise our saved images in the cache
  // are not included in the node object when the nodeapi is called
  dsm($form);
  dsm($form_state);
  if(!empty($form['#node']->imgupload_images) && is_array($form['#node']->imgupload_images)) {
    $form_state['values']['imgupload_images'] = $form['#node']->imgupload_images;
  }
  else if ($form_state['submitted'] == true) {
    // inactive form cache for broken items such as comment
    $cache_id = 'wysiwyg_imageupload_' . $form_state['values']['form_build_id'];
    $value = cache_get($cache_id, 'cache');
    dsm('submitid:'.$form_state['values']['form_build_id']);
    dsm($value);
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
