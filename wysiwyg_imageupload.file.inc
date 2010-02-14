<?php
// $Id$

# Copyright (c) 2010 Impressive.media
# Author: Eugen Mayer

/*
 * Uploading a image and calling the file_insert hooks.
 */
function _wysiwyg_imageupload_upload_file(&$form,&$form_state) {
  global $user;

  $dest = file_directory_path();
  $validators = array(
    'file_validate_is_image' => array()
    // TODO: Implement a administration interface for filesize settings
    // 'file_validate_size' => array($limits['file_size'], $limits['user_size']),
  );
  $file = NULL;
  if ( user_access('use wysiwyg image upload') && $file = file_save_upload('wysiwyg_imageupload_file', $validators, $dest,FILE_EXISTS_RENAME)) {
    foreach (module_implements('file_insert') as $module) {
      $function =  $module .'_file_insert';
      $function($file);
    }

    $file->list = 0;  // do not list the images
    $file->description = $file->filename;
    $file->weight = 0;
    $file->new = TRUE;
    $form['#node']->imgupload_images[$file->fid] = $file;
    $form['#value']['imgupload_images'][$file->fid] = $file;

    if (!@chmod($file->filepath, 0664)) {
      watchdog('wysiwyg_imageupload', 'Could not set permissons on destination file: %file', array('%file' => $file->filepath));
    }
  }

  return (object)$file;
}


/*
 * Share from the upload.module
 * Loading all files of the node
 */
function _wysiwyg_imageupload_load($node) {
  $files = array();

  if ($node->vid) {
    $result = db_query('SELECT * FROM {files} f INNER JOIN {wysiwyg_imageupload} r ON f.fid = r.fid WHERE r.vid = %d ORDER BY r.weight, f.fid', $node->vid);
    while ($file = db_fetch_object($result)) {
      $files[$file->fid] = $file;
    }
  }

  return $files;
}

/*
 * Share from the upload.module
 * Adds, removes or updates files in the image property.
 * This is actually the place where a file object get attached to a node, the
 * relation is saved in the wysiwyg_imageupload table
 */
function _wysiwyg_imageupload_save(&$node) {
  if (empty($node->imgupload_images) || !is_array($node->imgupload_images)) {
    return;
  }

  foreach ($node->imgupload_images as $fid => $file) {
    // Convert file to object for compatibility
    $file = (object)$file;

    // Remove file. Process removals first since no further processing
    // will be required.
    if (!empty($file->remove)) {
      db_query('DELETE FROM {wysiwyg_imageupload} WHERE fid = %d AND vid = %d', $fid, $node->vid);

      // If the file isn't used by any other revisions delete it.
      $count = db_result(db_query('SELECT COUNT(fid) FROM {upload} WHERE fid = %d', $fid));
      if ($count < 1) {
        file_delete($file->filepath);
        db_query('DELETE FROM {files} WHERE fid = %d', $fid);
      }

      // Remove it from the session in the case of new uploads,
      // that you want to disassociate before node submission.
      unset($node->imgupload_images[$fid]);
      // Move on, so the removed file won't be added to new revisions.
      continue;
    }

    // Create a new revision, or associate a new file needed.
    if (!empty($node->old_vid) || $file->new) {
      db_query("INSERT INTO {wysiwyg_imageupload} (fid, nid, vid, list, description, weight) VALUES (%d, %d, %d, %d, '%s', %d)", $file->fid, $node->nid, $node->vid, $file->list, $file->description, $file->weight);
      file_set_status($file, FILE_STATUS_PERMANENT);
    }
    // Update existing revision.
    else {
      db_query("UPDATE {wysiwyg_imageupload} SET list = %d, description = '%s', weight = %d WHERE fid = %d AND vid = %d", $file->list, $file->description, $file->weight, $file->fid, $node->vid);
      file_set_status($file, FILE_STATUS_PERMANENT);
    }
  }
}