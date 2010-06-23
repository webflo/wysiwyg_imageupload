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
    // Add stuff from files
    $this->additional_fields['fid'] = array('table' => 'files', 'field' => 'fid');
    $this->additional_fields['filename'] = array('table' => 'files', 'field' => 'filename');
    $this->additional_fields['filepath'] = array('table' => 'files', 'field' => 'filepath');
    $this->additional_fields['filemime'] = array('table' => 'files', 'field' => 'filemime');
    $this->additional_fields['filesize'] = array('table' => 'files', 'field' => 'filesize');
    $this->additional_fields['status'] = array('table' => 'files', 'field' => 'status');
    $this->additional_fields['timestamp'] = array('table' => 'files', 'field' => 'timestamp');
    // @TODO add stuff from revision.
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }
  
  function render($values) {
    dsm($values);
    return theme('wysiwyg_imageupload_render_image_entity', $object, array());
  }
}