<?php
// $Id$
/**
 * @file
 * Provide views integration for wysiwyg.
 */

/**
 * Implements hook_views_data().
 */
function wysiwyg_imageupload_views_data() {
  $data['wysiwyg_imageupload_entity']['table']['group']  = t('Wysiwyg ImageUpload');

  $data['wysiwyg_imageupload_entity']['table']['base'] = array(
    'field' => 'iid',
    'title' => t('Imageupload'),
    'help' => t("A image uploaded via wysiwyg imageupload."),
  );

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
    
    'comments' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );
  
  $data['wysiwyg_imageupload_entity']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the image.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wysiwyg_imageupload_entity']['imagecache'] = array(
    'title' => t('imagecache'),
    'help' => t('The imagecache of the image.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wysiwyg_imageupload_entity']['alignment'] = array(
    'title' => t('imagecache'),
    'help' => t('The imagecache of the image.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wysiwyg_imageupload_entity']['style'] = array(
    'title' => t('imagecache'),
    'help' => t('The imagecache of the image.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
        'temporary' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
        'description' => 'Is this file temporary.',
      ),

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

