<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _image_entity_node_info() {
  $items = array(
    'image_entity' => array(
      'name' => t('Image Entity'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
