<?php
// $Id$
/**
 * @file
 * Provide views integration for wysiwyg.
 */

/**
 * Implements hook_views_data().
 */
function wysiwyg_imagewysiwyg_imageupload_views_data() {
  $data['wysiwyg_imageupload_entity']['table']['group']  = t('Wysiwyg ImageUpload');

  $data['wysiwyg_imageupload_entity']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'files' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
  );

  return $data;
}

/**
 * Implements hook_vies_data_alter().
 */
function wysiwyg_imageupload_views_data_alter(&$data) {
}

/**
 * Implements hook_views_handlers().
 */
function wysiwyg_imageupload_views_handlers() {
  return array(
    'handlers' => array(
      // Fields
      // Filters
    ),
  );
}

