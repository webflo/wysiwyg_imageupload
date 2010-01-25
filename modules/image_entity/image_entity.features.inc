<?php

/**
 * Implementation of hook_content_default_fields().
 */
function image_entity_content_default_fields() {
  module_load_include('inc', 'image_entity', 'image_entity.defaults');
  $args = func_get_args();
  return call_user_func_array('_image_entity_content_default_fields', $args);
}

/**
 * Implementation of hook_node_info().
 */
function image_entity_node_info() {
  module_load_include('inc', 'image_entity', 'image_entity.features.node');
  $args = func_get_args();
  return call_user_func_array('_image_entity_node_info', $args);
}
