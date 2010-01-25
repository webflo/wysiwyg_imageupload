<?php
function wysiwyg_imageupload_imgupload_plugin() {
  $plugins = array();
  $plugins['imgupload'] = array(
    'title' => t('Image uploading'),
    'vendor url' => 'http://impressive-media.de',
    'icon file' => 'add_img.png',
    'icon title' => t('Insert or update an embedded image'),
    'settings' => array(
      'dialog' => array(
        'url' => '',
        'width' => 700,
        'height' => 300,
      ),
    ),
    // @todo Use a more granular way to validate contents for input formats.
    'extended_valid_elements' => array('img[class|src|border=0|alt|title|width|height|align|name|style]'),
  );
  return $plugins;
}
