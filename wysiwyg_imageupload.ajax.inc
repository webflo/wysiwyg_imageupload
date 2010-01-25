<?php
/*
 * Called through ajax(AHAH), uploading an image and replacing
 * the upload form with an image-details form, if successfully uploaded
 * Some of the code is shared from the upload.module
 */
function _wysiwyg_imageupload_upload_js($node_form_build_id) {
  // Load the form from the Form API cache.
  $cached_form_state = array();

  $cached_form = form_get_cache($node_form_build_id, $cached_form_state);
  unset($cached_form_state['storage']);

  // Handle new uploads, and merge tmp files into node-files.
  $imgDetails = _wysiwyg_imageupload_upload_file($cached_form,$form_state);

  form_set_cache($node_form_build_id, $cached_form, $cached_form_state);
  // remove all status messages
  //drupal_get_messages('status',true);
  if($imgDetails->filepath != "") {
    // Save the main form in the cache, adding the file.
    //$form_state['storage']['wysiwyg_upload']['files'][$imgDetails->fid] = $imgDetails;
    //form_set_cache($form_build_id, $form, $form_state);

    $output = theme('status_messages');
    // No errors, so lets replace the upload widget with the image details
    $title = $imgDetails->filename;

    // We save the fielpath in our cache, so we dont need to pass it later as a GET variable
    // (when we let the html object generate using ajax).
    $cacheid = uniqid();
    cache_set($cacheid,$imgDetails->filepath,'cache',CACHE_TEMPORARY);
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
    $output = $output.drupal_render($form);
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
   if (module_exists('imagecache')) {
    $filepath = cache_get($cacheID,'cache');
    $GLOBALS['devel_shutdown'] = FALSE;
    print theme('imagecache',$preset,$filepath->data, $filepath->data,$title);
    exit;
   }
}