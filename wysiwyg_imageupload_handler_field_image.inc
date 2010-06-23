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
  
  function query() {
//     dsm($this->query->ensure_table('files'));//, $this->query->get_join_data('files', $this->base_table)));
    parent::query();
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }
  
  function render($values) {
    $value = $values->{$this->field_alias};
    $object = new stdClass();
    $object->iid = $value;
    $object->imagecache = $values->{$this->aliases['imagecache']};
    $object->style = $values->{$this->aliases['style']};
    $object->title = $values->{$this->aliases['title']};
    $object->alignment = $values->{$this->aliases['alignment']};
    $object->fid = $values->{$this->aliases['fid']};
    $object->filename = $values->{$this->aliases['filename']};
    $object->filepath = $values->{$this->aliases['filepath']};
    $object->filesize = $values->{$this->aliases['filesize']};
    $object->filemime = $values->{$this->aliases['filemime']};
    $object->status = $values->{$this->aliases['status']};
    $object->timestamp = $values->{$this->aliases['timestamp']};
    // @TODO make the relationship working.
    $object = _wysiwyg_imageupload_load_inline_entity($value);
    
    return theme('wysiwyg_imageupload_render_image_entity', $object, array());
  }
}