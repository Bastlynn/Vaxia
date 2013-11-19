<?php
/**
   * @file
   * A Views' field handler for the player item shop.
   */
class item_shop_field_handler_add_item extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Render a Views form item placeholder.
    // This causes Views to wrap the View in a form.
    // Render a Views form item placeholder.
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Add to and alter the form created by Views.
   */
  function views_form(&$form, &$form_state) {
    // Create a container for our replacements
    $form[$this->options['id']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    // Iterate over the result and add our replacement fields to the form.
    foreach($this->view->result as $row_index => $row) {
      // Add a checkbox field to the form.
      $form[$this->options['id']][$row_index] = array(
        '#type' => 'checkbox',
        '#default_value' => 0,
        '#return_value' => 1,
      );
    }
  }

  /**
   * Form submit method.
   */
  function views_form_submit($form, &$form_state) {
    // Determine which nodes we need to update.
    $buy = array();
    // Iterate over the view result.
    foreach($this->view->result as $row_index => $row) {
      // Grab the correspondingly submitted form value.
      $value = $form_state['values'][$this->options['id']][$row_index];
      // If the submitted value is true, we're buying this one.
      if ($value) {
        $buy[$row->{$this->aliases['nid']}] = $value;
      }
    }
    // Get node vlaues form path.
    $node = node_load(arg(1));
    $lang = $node->language;
    // Get char from argument.
    $char = isset($node->nid) ? $node->nid : '';
    // Check values are ok.
    if (empty($char)) {
      return;
    }

    // Get character's owner.
    $user = user_load($node->uid);
    // Check values are ok.
    if (empty($user)) {
      return;
    }

    // Get realm from argument.
    $realm = isset($node->field_realm[$lang][0]['value']) ? $node->field_realm[$lang][0]['value'] : 'Vaxia';
    // Check values are ok.
    if (empty($realm)) {
      return;
    }

    // Get available silver from character.
    $silver = _character_sheet_get_xp($node->nid, 'silver');
    $silver_spent = _character_sheet_get_xp($node->nid, 'spent_silver');
    $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
    $available_silver = $silver - ($silver_spent + $silver_invested);
    // Available economy.
    $economy = isset($node->field_economy[$lang][0]['value']) ? $node->field_economy[$lang][0]['value'] : 0;
    $worth = _vaxia_calculate_worth($economy);
    // Grand total available.
    $total_worth = $available_silver + $worth;
    $silver = isset($total_worth) ? $total_worth : 0;
    // Check values are ok.
    if (empty($silver)) {
      return;
    }

    $item = node_load($row->nid);
    // Check the silver value for this item.
    $lang = $item->language;
    $this_value = $item->field_value[$lang][0]['value'];
    $this_value = str_replace(',', '', $this_value);
    $this_value = str_replace('.', '', $this_value);
    if (!is_numeric($this_value)) {
      return;
    }
    if ($silver < $this_value) {
      return;
    }

    // Grab the nodes we need to update and update them.
    $nodes = node_load_multiple(array_keys($buy));
    // Purchasing nodes.
    $total_cost = 0;
    foreach($nodes as $nid => $node) {
      $lang = $node->language;
      $this_value = trim($node->field_value[$lang][0]['value']);
      $this_value = str_replace(',', '', $this_value);
      $this_value = str_replace('.', '', $this_value);
      $total_cost += $this_value;
    }
    if ($silver < $total_cost) {
      $error = 1;
      drupal_set_message(t('You do not have enough silver for all of these items. ' .
        'Please update your selected items list. No silver charged.'));
      return;
    }

    // Still good? Add items.
    foreach($nodes as $nid => $node) {
      $lang = $node->language;
      if (!empty($char)) {
        $this_realm = $node->field_realm[$lang][0]['value'];
        $this_value = $node->field_value[$lang][0]['value'];
        $this_value = str_replace(',', '', $this_value);
        $this_value = str_replace('.', '', $this_value);
        $error = 0;
        if ($realm != $this_realm) {
          $error = 1;
          drupal_set_message(t('This item is not the same realm as the character. ' .
            ':title has NOT been added to this character. No silver charged.',
            array(':title' => $node->title)));
        }
        if (!is_numeric($this_value) ) {
          $error = 1;
          drupal_set_message(t('This item does not have a material cost. ' .
            ':title has NOT been added to this character. No silver charged.',
            array(':title' => $node->title)));
        }
        if ($silver < $this_value) {
          $error = 1;
          drupal_set_message(t('You do not have enough silver for this item. ' .
            ':title has NOT been added to this character. No silver charged.',
            array(':title' => $node->title)));
        }
        $error=1;
        if(!$error) {
          // Charge silver.
          _add_silver_submit_form_remove_silver($char, -1 * $this_value);
          // Create new node for the player, and character.
          $new_node = clone $node;
          unset($new_node->nid);
          unset($new_node->vid);
          $new_node->uid = $user->uid;
          $new_node->field_in_shop[$lang][0]['value'] = 0;
          $new_node->field_character_owner[$lang][0]['nid'] = $char;
          node_save($new_node);
          // Flush Entity Cache record.
          if (module_exists('entitycache')) {
            cache_clear_all('*', 'cache_entity_node', TRUE);
          }
          // Display message.
          drupal_set_message(t(':title has been added to this character. :silver charged.',
            array(':title' => $new_node->title, ':silver' => $this_value)));
        }
      }
    }
  }

}