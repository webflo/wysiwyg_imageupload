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

  $data['wysiwyg_imageupload_entity']['vid'] = array(
    'title' => t('Node'),
    'help' => t('The node the wysiwyg_imageuploaded file is attached to'),
    'relationship' => array(
      'label' => t('wysiwyg_imageupload'),
      'base' => 'node',
      'base field' => 'vid',
      // This allows us to not show this relationship if the base is already
      // node so users won't create circular relationships.
      'skip base' => array('node', 'node_revisions'),
    ),
  );

  $data['wysiwyg_imageupload_entity']['description'] = array(
    'title' => t('Description'),
    'help' => t('The description of the wysiwyg_imageuploaded file.'),
    'field' => array(
      'handler' => 'views_handler_field_upload_description',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wysiwyg_imageupload_entity']['list'] = array(
    'title' => t('Listed'),
    'help' => t('Whether or not the file is marked to be listed.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['wysiwyg_imageupload_entity']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('The weight, used for sorting.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}

/**
 * Implements hook_vies_data_alter().
 */
function wysiwyg_imageupload_views_data_alter(&$data) {
  $data['node']['wysiwyg_imageupload_entity_fid'] = array(
    'group' => t('Wysiwyg imageupload'),
    'title' => t('Attached files'),
    'help' => t('All files attached to a node with wysiwyg_imageupload.module.'),
    'real field' => 'vid',
    'field' => array(
      'handler' => 'wysiwyg_imageupload_handler_field_fid',
      'no group by' => TRUE,
    ),
    'filter' => array(
      'handler' => 'wysiwyg_imageupload_handler_filter_fid',
      'title' => t('Has attached files'),
      'type' => 'yes-no',
      'help' => t('Only display items with attached files. This can cause duplicates if there are multiple attached files.'),
    ),
    'relationship' => array(
      'title' => t('Attached files'),
      'relationship table' => 'wysiwyg_imageupload_entity',
      'relationship field' => 'fid',
      'base' => 'files',
      'base field' => 'fid',
      'handler' => 'views_handler_relationship',
      'label' => t('Files'),
    ),
  );
}

/**
 * Implements hook_views_handlers().
 */
function wysiwyg_imageupload_views_handlers() {
  return array(
    'handlers' => array(
      // Fields
      'wysiwyg_imageupload_handler_field_fid' => array(
        'parent' => 'views_handler_field_upload_fid',
      ),
      // Filters
      'wysiwyg_imageupload_handler_filter_fid' => array(
        'parent' => 'views_handler_filter_upload_fid',
      ),
    ),
  );
}

