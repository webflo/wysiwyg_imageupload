<?php
function imgassist_migrate_types() {
  static $nodetypes = NULL;

  if (!isset($nodetypes)) {
    $nodetypes = array();
    foreach (node_get_types() as $type => $info) {
      $nodetypes['node/' . $type] = t('Node: ') . $info->name;
    }
    asort($nodetypes);
  }
  return $nodetypes;
}


function imgassist_migrate_prepare_node($node,$tblinfo,$row) {
  $node->title ="foo";
  watchdog(WATCHDOG_WARNING, 'node converted:'.$node->nid);
}