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
      'left_table' => 'wysiwyg_imageupload_entity_revision'
      'left_field' => 'iid',
      'field' => 'iid',
    ),
    'node_revisions' => array(
      'left_table' => 'wysiwyg_imageupload_entity_revision'
      'left_field' => 'iid',
      'field' => 'iid',
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
  
  // This displays a image, will all goodies, therefore it needs a extra handler.
  $data['wysiwyg_imageupload_entity']['iid'] = array(
    'title' => t('Image'),
    'help' => t('The image.'),
    'field' => array(
      'handler' => 'wysiwyg_imageupload_handler_field_image',
      'click sortable' => FALSE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'base' => 'files',
      'field' => 'fid',
//       'label' => t('Related term'),
    ),
    // @TODO Perhaps someone needs a filter/sort here, but i cannot imagine this.
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
    'title' => t('alignment'),
    'help' => t('The alignment of the image.'),
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
    'title' => t('Style'),
    'help' => t('The Style of the image.'),
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
  
  $data['wysiwyg_imageupload_entity']['temporary'] = array(
    'title' => t('temporary'),
    'help' => t('The temporary of the image.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Here comes the cool stuff, relationships!!
  $data['wysiwyg_imageupload_entity']['fid'] = array(
    'title' => t('Files'),
    'help' => t('The uploaded file'),
    'relationship' => array(
      'label' => t('File'),
      'base' => 'files',
      'base field' => 'fid',
    ),
  );
  $data['wysiwyg_imageupload_entity']['nid'] = array(
    'title' => t('Node'),
    'help' => t('The node the uploaded file is attached to'),
    'relationship' => array(
      'label' => t('Node'),
      'base' => 'node',
      'base field' => 'nid',
    ),
  );
    $data['wysiwyg_imageupload_entity']['cid'] = array(
    'title' => t('Comment'),
    'help' => t('The comment the uploaded file is attached to'),
    'relationship' => array(
      'label' => t('Comment'),
      'base' => 'comments',
      'base field' => 'cid',
    ),
  );

  $data['wysiwyg_imageupload_entity_revision']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  return $data;
}

/**
 * Implements hook_vies_data_alter().
 */
function wysiwyg_imageupload_views_data_alter(&$data) {
  $data['files']['join']['table']['wysiwyg_imageupload_entity'] = array(
    'left_field' => 'fid',
    'field' => 'fid',
  );
}

/**
 * Implements hook_views_handlers().
 */
function wysiwyg_imageupload_views_handlers() {
  return array(
    'handlers' => array(
      // Fields
      'wysiwyg_imageupload_handler_field_image' => array(
        'parent' => 'views_handler_field',
      ),
      // Filters
    ),
  );
}

