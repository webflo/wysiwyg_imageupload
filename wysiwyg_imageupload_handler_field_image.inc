<?php
// $Id: $

class wysiwyg_imageupload_handler_field_image extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
    // Add stuff from wysiwyg
    $this->additional_fields['imagecache'] = array('table' => 'wysiwyg_imageupload_entity', 'field' => 'imagecache');
    $this->additional_fields['style'] = array('table' => 'wysiwyg_imageupload_entity', 'field' => 'style');
    $this->additional_fields['title'] = array('table' => 'wysiwyg_imageupload_entity', 'field' => 'title');
    $this->additional_fields['alignment'] = array('table' => 'wysiwyg_imageupload_entity', 'field' => 'alignment');
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }
  
  function render($values) {
    return theme('wysiwyg_imageupload_render_image_entity', $object, array());
  }
}