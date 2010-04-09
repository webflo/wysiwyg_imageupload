<?php
// $Id$

/**
 * Filter by whether or not a node has attached files from the wyiswyg_imageupload module
 */
class wysiwyg_imageupload_handler_filter_fid extends views_handler_filter_upload_fid {

  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "(SELECT COUNT(*) FROM {wysiwyg_imageupload} u WHERE u.vid = $this->table_alias.$this->real_field) " . (empty($this->value) ? '=' : '<>') . " 0");
  }
}

