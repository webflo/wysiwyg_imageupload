<?php
// $Id$

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer
/*
 * Providing a administration interface for tagging.
 */
function wysiwyg_imageupload_admin_settings() {
  $form['wysiwyg_imageupload_imgstyles'] = array(
    '#type' => 'textfield',
    '#title' => t('Image style'),
    '#description' => t('Define which CSS styles the user can chose when adding an image. Please do not define floating or size in that CSS classes, as this is handled seperately. Your calls must have the name "imgupl_style_*". You can give the style a descriptive name to let it handle more easily. The syntax is "Descriptive Name=imgupl_style_XYC,Other Descriptive name=imgupl_style_ZXC.." while imgupl_style_* is a css class'),
    '#default_value' => variable_get('wysiwyg_imageupload_imgstyles', '')
  );

  $presets = imagecache_presets();
  $styles = array();
  foreach ($presets as $preset) {
    // We can use the presetid here (http://drupal.org/node/694188).
    $styles[$preset['presetname']] = t($preset['presetname']);
  }

  $form['wysiwyg_imageupload_allow_presets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allowed preset'),
    '#description' => t('Only the selected presets are allowed to be chosen in the dialog')
  );

 $form['wysiwyg_imageupload_allow_presets']['wysiwyg_imageupload_presets_whitelist'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Preset'),
    '#default_value' => variable_get('wysiwyg_imageupload_presets_whitelist', $styles),
    '#options' => $styles
  );

  return system_settings_form($form);
}