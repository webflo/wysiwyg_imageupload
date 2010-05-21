<?php
// $Id$

/**
 * Field handler to provide a list of fids.
 */
class wyswiyg_imageupload_handler_field_fid extends views_handler_field_upload_fid {
  function pre_render($values) {
    $vids = array();
    $this->items = array();

    foreach ($values as $result) {
      $vids[] = $result->{$this->field_alias};
    }

    if ($vids) {
      // Support "only listed files" option.
      $where = '';
      if (!empty($this->options['only_listed'])) {
        $where = " AND u.list <> 0";
      }
      $result = db_query("SELECT u.vid, u.fid, f.filename, f.filepath, f.filesize, f.filemime, u.description FROM {wysiwyg_imageupload_entity} u LEFT JOIN {files} f ON f.fid = u.fid WHERE u.vid IN (" . implode(', ', $vids) . ")$where ORDER BY u.weight");
      while ($file = db_fetch_array($result)) {
        $file['filename'] = check_plain($file['filename']);
        $file['filemime'] = check_plain($file['filemime']);
        $file['description'] = check_plain($file['description']);
        $file['filesize'] = format_size($file['filesize']);
        $file['filepath'] = file_create_url($file['filepath']);
        if (!empty($this->options['link_to_file']) ) {
          $file['make_link'] = TRUE;
          $file['path'] = $file['filepath'];
        }
        $this->items[$file['vid']][$file['fid']] = $file;
      }
    }
  }
}
