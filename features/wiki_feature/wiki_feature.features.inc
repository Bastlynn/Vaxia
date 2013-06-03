<?php
/**
 * @file
 * wiki_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wiki_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function wiki_feature_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function wiki_feature_node_info() {
  $items = array(
    'wiki_page' => array(
      'name' => t('Wiki page'),
      'base' => 'node_content',
      'description' => t('A page in the wiki'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function wiki_feature_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Wiki workflow
  $workflows['Wiki workflow'] = array(
    'name' => 'Wiki workflow',
    'tab_roles' => '3,4',
    'options' => 'a:3:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => '-50',
        'sysid' => '1',
        'status' => '1',
      ),
      1 => array(
        'state' => 'open',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
      2 => array(
        'state' => 'locked',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'open',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'locked',
      ),
      2 => array(
        'roles' => 'administrator,administrator-vaxia',
        'state' => 'open',
        'target_state' => 'locked',
      ),
      3 => array(
        'roles' => 'administrator,administrator-vaxia',
        'state' => 'locked',
        'target_state' => 'open',
      ),
    ),
  );

  return $workflows;
}