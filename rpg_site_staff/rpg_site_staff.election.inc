<?php
/**
 * @file A file specifically for election related code.
 */

/**
 * Cron handler for elections.
 */
function _rpg_site_staff_cron_election() {
  $last_run = variable_get('proposal_cron', 0);
  $yesterday = strtotime('-1 day');
  if ($last_run < $yesterday) {
    $today = date('m/d/Y');
    $today = strtotime($today . ' 00:00:01'); // We always want to plan to run just after midnight.
    variable_set('proposal_cron', $today);
    $election_day = _rpg_site_staff_check_election_day();
    if ($election_day) {
      // This is an official election! Time to get things sorted out.
      // First remove current heads from positions, put admin in for anon mailer.
      $head_role = variable_get('rpg_site_staff_head_role', '');
      db_query('DELETE FROM {users_roles} WHERE rid = :rid',
        array(':uid' => $winner->uid, ':rid' => $head_role));
      $data = array('uid' => 1, 'rid' => $head_role);
      drupal_write_record('users_roles', $data);
      // And start the election.
      _rpg_site_staff_trigger_election();
    }
  }
}

/**
 * Helper function. Check if today is election day.
 */
function _rpg_site_staff_check_election_day() {
  // Grab the info about the last election and when / how to run it.
  $last_held = variable_get('rpg_site_staff_election_day', '5/1/2013');
  $rate = variable_get('rpg_site_staff_rate', '1 year');
  // Based on the last election, get the next election in a standard format.
  $last_election = strtotime($last_held);
  $next_election = date('m/d/Y', strtotime($rate, $last_election));
  // Calculate today, see if it's the same values in a standard format.
  $today = date('m/d/Y', strtotime('now'));
  // Do they match? Mark today in the variables.
  if ($today == $next_election) {
    variable_set('rpg_site_staff_election_day', date('m/d/Y'));
    return TRUE;
  }
  return FALSE;
}

/**
 * Add election fields to the settings form.
 */
function _rpg_site_staff_admin_election_settings(&$form, &$form_state) {
  $form['elections'] = array(
    '#type' => 'fieldset',
    '#title' => t('Election settings'),
    '#collapsible' => TRUE,
  );
  $form['elections']['rpg_site_staff_election_day'] = array(
    '#title' => t('Last election day'),
    '#description' => t('The last-held election day for calculating further elections.'),
    '#type' => 'textfield',
    '#disabled' => TRUE,
    '#default_value' => variable_get('rpg_site_staff_election_day', '5/1/2013'),
  );
  $form['elections']['rpg_site_staff_rate'] = array(
    '#title' => t('Election rate'),
    '#description' => t('How often to hold elections. Use timeframe formats strtotime will recognize.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('rpg_site_staff_rate', '1 year'),
  );
  $form['elections']['rpg_site_staff_term_limits'] = array(
    '#title' => t('Term limits'),
    '#description' => t('Use term limits when calculating eligable users for positions.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('rpg_site_staff_term_limits', TRUE),
  );
  $form['elections']['rpg_site_staff_term_limit_limits'] = array(
    '#title' => t('Term limit'),
    '#description' => t('How many consecutive (not lifetime) terms can a user serve in the same position.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('rpg_site_staff_term_limit_limits', 2),
  );
  $form['elections']['rpg_site_staff_run_roles'] = array(
    '#title' => t('Election run role'),
    '#description' => t('These roles are allowed to run.'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' =>  user_roles(),
    '#default_value' => variable_get('rpg_site_staff_run_roles', array()),
  );
  $form['elections']['rpg_site_staff_head_role'] = array(
    '#title' => t('Head role'),
    '#description' => t('This role is considered the head role. It recieves anonymous emails.'),
    '#type' => 'select',
    '#options' =>  user_roles(),
    '#default_value' => variable_get('rpg_site_staff_head_role', ''),
  );
  $vid = variable_get('forum_nav_vocabulary', 0);
  $options = array();
  if ($terms = taxonomy_get_tree($vid)) {
    foreach ($terms as $term) {
      $options[$term->tid] = str_repeat('-', $term->depth) . $term->name;
    }
  }
  $form['elections']['rpg_site_staff_default_forum'] = array(
    '#title' => t('Election forum'),
    '#description' => t('The default forum to post the election in.'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('rpg_site_staff_default_forum', ''),
  );
  $intro_text = t('Each year Vaxia has an election for the key positions on our site. ' .
    'This is so that we can always be sure that the current leadership is connected to ' .
    'and trusted by our players.');
  $form['elections']['rpg_site_staff_election_intro'] = array(
    '#title' => t('Election intro'),
    '#description' => t('Election introduction text.'),
    '#type' => 'textarea',
    '#default_value' => variable_get('rpg_site_staff_election_intro', $intro_text),
  );
  $available = implode("\n", array('Setting Head', 'System Head') );
  $form['elections']['rpg_site_staff_positions'] = array(
    '#title' => t('Available positions'),
    '#description' => t('List postitions avaiblae on the site, one per row.'),
    '#type' => 'textarea',
    '#default_value' => variable_get('rpg_site_staff_positions', $available),
  );
  $form['rpg_site_staff_emergency_election'] = array(
    '#type' => 'submit',
    '#value' => t('Hold an election off-cycle'),
    '#submit' => array('_rpg_site_staff_election_submit'),
  );
  $form['#submit'][] = '_rpg_site_staff_positions_submit';
}

/**
 * Submit callback, add user fields for each defined position.
 */
function _rpg_site_staff_election_submit($form, &$form_state) {
  // This is an official election! Time to get things sorted out.
  // First remove current heads from positions, put admin in for anon mailer.
  $head_role = variable_get('rpg_site_staff_head_role', '');
  db_query('DELETE FROM {users_roles} WHERE rid = :rid',
    array(':uid' => $winner->uid, ':rid' => $head_role));
  $data = array('uid' => 1, 'rid' => $head_role);
  drupal_write_record('users_roles', $data);
  // And start the election.
  _rpg_site_staff_trigger_election();
}

/**
 * Submit callback, add user fields for each defined position.
 */
function _rpg_site_staff_positions_submit($form, &$form_state) {
  // Grab the old positions.
  // Delete the old fields. TODO: For now, leave this to the admin to delete the old ones.
  // Grab the new positions from the settings.
  $available = $form_state['values']['rpg_site_staff_positions'];
  $available = explode("\n", $available);
  // Add the new position.
  foreach ($available as $position) {
    // Get the name.
    $position = trim($position);
    $field_key_name = str_replace(' ', '_', strtolower($position));
    $existing_field = field_info_field('field_staff_' . $field_key_name);
    if (empty($existing_field)) {
      // Set up the data arrays.
      $field = array(
        'active' => '1',
        'cardinality' => '1',
        'deleted' => '0',
        'entity_types' => array(),
        'field_name' => 'field_staff_' . $field_key_name,
        'foreign keys' => array(),
        'indexes' => array(
          'value' => array(
            0 => 'value',
          ),
        ),
        'locked' => '0',
        'module' => 'list',
        'settings' => array(
          'allowed_values' => array(
            0 => 'No',
            1 => 'Yes',
          ),
          'allowed_values_function' => '',
        ),
        'translatable' => '0',
        'type' => 'list_boolean',
      );
      $instance = array(
        'bundle' => 'user',
        'default_value' => array(
          0 => array(
            'value' => 1,
          ),
        ),
        'deleted' => '0',
        'description' => t('I would like to be considered eligible for ' . $position . ' positions during elections.'),
        'display' => array(
          'default' => array(
            'label' => 'inline',
            'settings' => array(),
            'type' => 'list_boolean',
            'weight' => '6',
          ),
        ),
        'entity_type' => 'user',
        'field_name' => 'field_staff_' . $field_key_name,
        'label' => t('Eligible for ' . $position . ''),
        'required' => 0,
        'settings' => array(
          'user_register_form' => 0,
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'options',
          'settings' => array(
            'display_label' => 1,
          ),
          'type' => 'options_onoff',
        ),
      );
      // Create the field.
      field_create_field($field);
      // Create the instance.
      field_create_instance($instance);
      // Add the field to the election group form.
      $group = field_group_info_groups('user', 'user', 'form', TRUE);
      $group = $group['group_election_settings'];
      $group->children[] = $field_key_name;
      field_group_group_save($group);
      // Add the field to the election group display.
      $group = field_group_info_groups('user', 'user', 'default', TRUE);
      $group = $group['group_voting'];
      $group->children[] = $field_key_name;
      field_group_group_save($group);
      // Update all existing users to have this field automatically turned on.
      db_query('REPLACE INTO {field_data_field_staff_' . $field_key_name . '} ' .
        '(entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_staff_' . $field_key_name . '_value) ' .
        'SELECT \'user\', \'user\', 0, u.uid, u.uid, \'und\', 0, 1 FROM {users} u WHERE u.status = 1');
      db_query('REPLACE INTO {field_revision_field_staff_' . $field_key_name . '} ' .
        '(entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_staff_' . $field_key_name . '_value) ' .
        'SELECT \'user\', \'user\', 0, u.uid, u.uid, \'und\', 0, 1 FROM {users} u WHERE u.status = 1');
      // Output message.
      drupal_set_message('Created user field ' . $field_key_name);
    }
  }
  // Flush Entity Cache record.
  if (module_exists('entitycache')) {
    cache_clear_all('*', 'cache_entity_node', TRUE);
  }
}

/**
 * Create an election set with all relevant data.
 */
function _rpg_site_staff_trigger_election($title = '', $positions = array(), $election = array()) {
  $title = !empty($title) ? $title : t('Site Election');
  $node->title = date('m/d/Y') . ' ' . t('Site Election');
  $node->type = 'proposal';
  $node->uid = 1;
  $node->language = 'und';
  $tid = variable_get('rpg_site_staff_default_forum', '');
  $node->taxonomy_forums[ $new_election->language ][0]['tid'] = !empty($tid) ? $tid : '';
  node_save($node);
  $node = node_load($node->nid);
  if (!empty($node)) {
    _rpg_site_staff_setup_election_proposal($node, $postion, $election);
  }
}

/**
 * Helper function. Create an election form with the right settings.
 */
function _rpg_site_staff_setup_election_proposal($new_election, $set_positions = array(), $election = array()) {
  $output = '';
  $output .= '<h3><b>Election</b></h3>';
  $intro_text = t('Each year Vaxia has an election for the key positions on our site. ' .
    'This is so that we can always be sure that the current leadership is connected to ' .
    'and trusted by our players.');
  $output .= '<p>' . variable_get('rpg_site_staff_election_intro', $intro_text) . '</p>';
  $output .= '<p>Available positions are:</p>';
  $output .= '<ul>';
  // Figure out the positions.
  // If we've not been handed the election ties...
  $keys = array();
  if (!empty($set_positions)) {
    foreach ($set_positions as $position) {
      $position = trim($position);
      $keys[$position] = $position;
    }
  }
  else {
    $available = variable_get('rpg_site_staff_positions', "Setting Head\nSystem Head");
    $available = explode("\n", $available);
    foreach ($available as $position) {
      $position = trim($position);
      $keys[$position . ' A'] = $position . ' A';
      $keys[$position . ' B'] = $position . ' B';
    }
  }
  // Set up the output based on the key.
  foreach ($keys as $key) {
    $output .= '<li>' . check_plain($key) . '</li>';
  }
  $output .= '</ul>';
  $output .= '<p>Please take a moment to review the candidates for this ' .
    'year and select those you feel to be the best choices for the available positions.</p>';
  // Set the webform body and basic configurations.
  $new_election->body[$new_election->language][0]['value'] = $output;
  $new_election->webform['confirmation'] = t('Thank you for participating!');
  $new_election->webform['confirmation_format'] = 'plain_text';
  $new_election->webform['submit_limit'] = 1;
  // Save the node.
  node_save($new_election);
  // Set approving roles for users based on all roles with 'can vote on elections' permission.
  $rpg_site_staff_voting_roles = user_roles(FALSE, 'can vote on elections');
  $record = db_query('DELETE FROM {webform_roles} WHERE nid = :nid', array(':nid' => $new_election->nid));
  foreach ($rpg_site_staff_voting_roles as $rid => $role) {
    if ($role) {
      db_insert('webform_roles')->fields(array('nid' => $new_election->nid, 'rid' => $rid))->execute();
    }
  }
  // Set the components for the form.
  _rpg_site_staff_setup_election_proposal_components($new_election, $keys);
  // And now we mark this proposal as having been created in our site as an election object.
  $data = array(
    'nid' => $new_election->nid,
    'positions' => $keys,
  );
  $data = array_merge( (array) $election, $data); // Merge any existing election info.
  if (isset($data['eid'])) {
    unset($data['eid']); // It's a new election, but same cycle, same positions.
  }
  // If this is a runoff it's on the same cycle after all.
  $data = _rpg_site_staff_update_election($data);
  // And set a message to our posts.
  drupal_set_message(t('A new election has been created at !link.',
    array('!link' => l($new_election->title, 'node/' . $new_election->nid))));
}

/**
 * Helper function. Create the components for an election form.
 */
function _rpg_site_staff_setup_election_proposal_components($new_election, $keys) {
  // Include the webform module that allows components to be created.
  module_load_include('inc', 'webform', 'includes/webform.components');
  // Create the webform fields for possible elected.
  $weight = 0;
  foreach ($keys as $key => $title) {
    $field_key_name = str_replace(' ', '_', strtolower(substr($key, 0, -2))); // Trim end, fied field.
    // Gather the user info.
    $roles = variable_get('rpg_site_staff_run_roles', array());
    $roles = array_values($roles);
    $system_eligible_names = db_query('SELECT u.name, u.uid FROM {users} u ' . 
      'LEFT JOIN {users_roles} ur ON u.uid = ur.uid ' .
      'LEFT JOIN {field_data_field_staff_' . $field_key_name . '} f ON u.uid = f.entity_id ' .
      'WHERE ur.rid IN (:voting_roles) ' .
      'AND f.field_staff_' . $field_key_name . '_value = 1 ' .
      'ORDER BY u.name',
      array(':voting_roles' => $roles));
    $system_eligible = '';
    $term_limits = variable_get('rpg_site_staff_term_limits', TRUE);
    $term_limits_limit = variable_get('rpg_site_staff_term_limit_limits', 2);
    foreach ($system_eligible_names as $system_eligible_name) {
      // Check term limtis
      $eligable = TRUE;
      if ($term_limits) {
        // Check for this position key if the user has the ok or if they need to lay off for a bit.
        $this_position = substr($key, 0, -2); // Clip the last two characters off the end.
        $eligable = !_rpg_site_staff_is_at_term_limit( $system_eligible_name->uid, $term_limits_limit, $this_position);
      }
      // If we're still ok, post it.
      if($eligable) {
        $system_eligible .= $system_eligible_name->name . '|' . $system_eligible_name->name . "\n";
      }
    }
    $component = array();
    $component['nid'] = $new_election->nid;
    $component['pid'] = 0;
    $component['form_key'] = $key;
    $component['name'] = $title;
    $component['type'] = 'select';
    $component['mandatory'] = 1;
    $component['extra'] = array(
      'description' => '',
      'items' => $system_eligible,
      'multiple' => '0',
      'aslist' => '1',
    );
    $component['weight'] = $weight;
    $weight++;
    webform_component_insert($component);
  }
  // Add additional fields or comments.
  $component = array();
  $component['nid'] = $new_election->nid;
  $component['pid'] = 0;
  $component['form_key'] = 'comments';
  $component['name'] = t('Additional Comments and Feedback');
  $component['type'] = 'textarea';
  $component['mandatory'] = 0;
  $component['extra'] = array(
    'description' => '',
    'items' => '',
    'multiple' => '0',
    'aslist' => '0',
    );
  $component['weight'] = $weight;
  webform_component_insert($component);
}

/**
 * Handle when we close an election.
 */
function _rpg_site_staff_election_check_close($node) {
  // Check if we are closed. If we aren't an election this returns nothing.
  $election = __rpg_site_staff_get_election_by_nid($node->nid);
  if ($election && $election->closed == -1 && $node->webform['status'] == FALSE) {
    // And now we mark this proposal as having been created in our site as an election object.
    $election->closed = strtotime('now');
    $election->winners = array();
    $winners = _rpg_site_staff_get_elected($node);
    $head_role = variable_get('rpg_site_staff_head_role', '');
    // Closed? Good - now we should be getting some winners in - if so, yank user one out of head.
    if (!empty($winners['winners'])) {
      db_query('DELETE FROM {users_roles} WHERE rid = :rid',
        array(':uid' => 1, ':rid' => $head_role));
    }
    // Now process the winners, marking them for terms etc as needed.
    $these_winners = array();
    foreach ($winners['winners'] as $position => $winner) {
      // Add the winner to the heads role.
      db_query('DELETE FROM {users_roles} WHERE uid = :uid AND rid = :rid',
        array(':uid' => $winner->uid, ':rid' => $head_role));
      $data = array('uid' => $winner->uid, 'rid' => $head_role);
      drupal_write_record('users_roles', $data);
      // Note the position fore recording.
      $these_winners[$position] = $winner->uid;
      // And for each winner, mark that they've been assigned for this cycle to this position.
      $this_winner = array();
      $this_winner['uid'] = $winner->uid;
      $this_winner['cycle'] = $election->cycle;
      $this_winner['position'] = substr($position, 0, -2); // Clip the last two characters off the end.
      $this_winner = _rpg_site_staff_mark_winner($this_winner);
    }
    // Add to the election information recorded so far.
    $election->winners = $these_winners;
    $election = _rpg_site_staff_update_election($election);
    // Now we have to address the ties. We need to run a tie breaker election with JUST these postions.
    // So we open ANOTHER election.
    if (!empty($winners['ties'])) {
      $winners['ties'] = array_unique($winners['ties']);
      _rpg_site_staff_trigger_election(t('Vaxia Site Run-off Election'), $winners['ties'], $election);
    } // End tie handling.
  }
}

/**
 * Helper function, given an election node figure out who the winners are.
 */
function _rpg_site_staff_get_elected($node) {
  // Include the webform file we need.
  module_load_include('inc', 'webform', 'includes/webform.submissions');
  // Get submission results.
  $submissions = webform_get_submissions($node->nid);
  $results = array();
  foreach ($submissions as $sid => $submission) {
    foreach ($submission->data as $cid => $submission_answer) {
      $answer = $submission_answer['value'][0];
      $results[$cid][$answer] = !empty($results[$cid][$answer]) ? $results[$cid][$answer] + 1 : 1;
    }
  }
  // Map components to head positions.
  $elected = array();
  foreach ($node->webform['components'] as $cid => $component) {
    if ($component['type'] == 'select') {
      $voted_for = $results[$cid];
      $max = 0;
      $winner = array();
      foreach ($voted_for as $user_name => $votes) {
        if ($votes == $max) {
          $max = $votes;
          $winner[] = $user_name;
        }
        elseif ($votes > $max) {
          $max = $votes;
          $winner = array($user_name);
        }
      }
      $elected[ $component['name'] ] = $winner;
    }
  }
  // Now we look for ties and set user ids.
  $tie_positions = array();
  foreach ($elected as $position => $winners) {
    if (count($winners) > 1) {
      // WE HAVE A TIE.
      $tie_positions[$position] = 1;
      foreach ($winners as $index => $winner_name) {
        $this_winner = user_load_by_name($winner_name);
        $elected['tied_winners'][$position][$index] = $this_winner;
      }
    }
    else {
      unset( $tie_positions[$position] );
      $this_winner = user_load_by_name($winners[0]);
      $elected['winners'][$position] = $this_winner;
    }
  }
  // We have a tie. This is a not good thing.
  if (!empty($tie_positions)) {
    watchdog('rpg_site_staff', 'This election has a TIE. Another follow-up should be run.');
    drupal_set_message(t('This election has a TIE. Another follow-up should be run.'));
    $elected['ties'] = array_keys($tie_positions);
  }
  return $elected;
}

/**
 * CRUD function. Set or create election info for a node.
 */
function _rpg_site_staff_update_election($election) {
  $data = (array) $election;
  if (!isset($data['nid'])) {
    return FALSE; // We must have a nid for this election data.
  }
  $old_data = __rpg_site_staff_get_election_by_nid($data['nid']);
  if ($old_data) {
    $data = array_merge( (array) $old_data, $data);
    $data['positions'] = serialize($data['positions']);
    $data['winners'] = serialize($data['winners']);
    drupal_write_record('rpg_site_staff_election_nodes', $data, 'eid');
  }
  else {
    // New election post. Set the new cycle and open time.
    $cycle = _rpg_site_staff_get_last_cycle();
    $cycle++;
    $data['cycle'] = !empty($data['cycle']) ? $data['cycle'] : $cycle;
    $data['opened'] = strtotime('now');
    $data['closed'] = -1;
    $data['winners'] = array();
    $data['positions'] = serialize($data['positions']);
    $data['winners'] = serialize($data['winners']);
    // Write it.
    drupal_write_record('rpg_site_staff_election_nodes', $data);
  }
  // Data returned on pointer, but just in case.
  return (object) $data;
}

/**
 * CRUD function. Get election info for an election.
 */
function _rpg_site_staff_get_election($eid) {
  if (empty($eid)) {
    return FALSE; // Bail if error.
  }
  $election = db_query('SELECT * FROM {rpg_site_staff_election_nodes} WHERE eid = :eid',
    array(':eid' => $eid))->fetchObject();
  if (empty($election->eid)) {
    return FALSE; // Bail if error.
  }
  $data->positions = unserialize($data->positions);
  $data->winners = unserialize($data->winners);
  return $election;
}

/**
 * CRUD function. Get election info for a node.
 */
function __rpg_site_staff_get_election_by_nid($nid) {
  if (empty($nid)) {
    return FALSE; // Bail if error.
  }
  $election = db_query('SELECT * FROM {rpg_site_staff_election_nodes} WHERE nid = :nid',
    array(':nid' => $nid))->fetchObject();
  if (empty($election->eid)) {
    return FALSE; // Bail if error.
  }
  if (!empty($election->positions)) {
    $election->positions = unserialize($election->positions);
  }
  if (!empty($election->winners)) {
    $election->winners = unserialize($election->winners);
  }
  return $election;
}

/**
 * CRUD function. Delete election info for a node.
 */
function _rpg_site_staff_delete_election($eid) {
  if (empty($eid)) {
    return FALSE; // Bail if error.
  }
  $election = db_query('DELETE FROM {rpg_site_staff_election_nodes} WHERE eid = :eid',
    array(':eid' => $eid))->fetchObject();
}

/**
 * CRUD function. Delete election info for a node.
 */
function __rpg_site_staff_delete_election_by_nid($nid) {
  if (empty($nid)) {
    return FALSE; // Bail if error.
  }
  db_query('DELETE FROM {rpg_site_staff_election_nodes} WHERE nid = :nid',
    array(':nid' => $nid));
}

/**
 * DB function. Get the current max cycle.
 */
function _rpg_site_staff_get_last_cycle() {
  $cycle = db_query('SELECT MAX(cycle) FROM {rpg_site_staff_election_nodes}')->fetchField();
  return !empty($cycle) ? $cycle : 1;
}

/**
 * CRUD function. Set or create election info for a node.
 */
function _rpg_site_staff_mark_winner($winner) {
  $data = (array) $winner;
  if (empty($data['uid']) || empty($data['cycle']) || empty($data['position'])) {
    return FALSE; // We must have a nid for this election data.
  }
  // New election post. Set the new cycle and open time.
  drupal_write_record('rpg_site_staff_election_winners', $data);
  // Data returned on pointer, but just in case.
  return (object) $data;
}

/**
 * DB function. Get election info for an election.
 */
function __rpg_site_staff_get_election_winner($uid, $cycle, $postion) {
  if (empty($eid)) {
    return FALSE; // Bail if error.
  }
  $election = db_query('SELECT * FROM {rpg_site_staff_election_winners} ' .
    'WHERE uid = :uid ' .
    'AND cycle = :cycle ' .
    'AND position = :position',
    array(':uid' => $uid, ':cycle' => $cycle, ':position' => $position))->fetchObject();
  if (empty($election->wid)) {
    return FALSE; // Bail if error.
  }
  return $election;
}

/**
 * CRUD function. Delete election info for a node.
 */
function _rpg_site_staff_delete_winners_by_uid($uid) {
  if (empty($uid)) {
    return FALSE; // Bail if error.
  }
  db_query('DELETE FROM {rpg_site_staff_election_winners} WHERE uid = :uid',
    array(':uid' => $uid));
}

/**
 * DB function. Figure out if this user is at term limit.
 */
function _rpg_site_staff_is_at_term_limit($uid, $term_limit, $position) {
  $current_cycle = _rpg_site_staff_get_last_cycle();
  $cycle_limit = $current_cycle - $term_limit;
  $times_held = db_query('SELECT COUNT(uid) AS held FROM {rpg_site_staff_election_winners} ' .
    'WHERE uid = :uid ' . 
    'AND position = :position ' .
    'AND cycle > :cycle',
    array(':uid' => $uid, ':position' => $position, ':cycle' => $cycle_limit))->fetchField();
  $times_held = !empty($times_held) ? $times_held : 0;
  if ($times_held < $term_limit) {
    return FALSE;
  }
  return TRUE;
}