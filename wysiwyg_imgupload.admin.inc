<?php
/*
 * Providing a administration interface for tagging.
 */
function wysiwyg_imageupload_admin_settings() {
  $form['wysiwyg_imageupload_imgstyles'] = array(
      '#type' => 'textfield',
      '#title' => t('Image style'),
      '#description' => t('Define which CSS styles the user can chose when adding an image. Please do not define floating or size in that CSS classes, as this is handled seperately. You can give the style a descriptive name to let it handle more easily. The syntax is "Descriptive Name=imgupl_style_XYC,Other Descriptive name=imgupl_style_ZXC.." while imgupl_style_* is a css class'),
      '#default_value' => variable_get('wysiwyg_imageupload_imgstyles', '')
  );

  return system_settings_form($form);
}
