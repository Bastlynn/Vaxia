/**
 * @file
 * Injects dice ruling buttons into the UI.
 */
Drupal.behaviors.diceRuler = {
  attach: function(context) { (function($) {

  // A variable to store the last slot we used or set.
  var last_slot = 0;
  var vs_slot = 0;

  // Setup the ruler as needed.
  function setupRuler() {
    // Show the dice ruler.
    $('#dice-ruler-form').show();
    // On page load, inject the label into place in the DOM.
    var rule_buttons = '<span class="dice_ruler_label dice_ruler_label_a">As A</span>' +
      '<span class="dice_ruler_label dice_ruler_label_b">As B</span>';
    $('.dice_ruled').prepend(rule_buttons);
    // First set of buttons.
    var rule_buttons = '<ul class="dice_rule dice-ruler-buttons dice-ruler-row-buttons"> ' +
      '<li class="dice-ruler-button dice-ruler-a dice-ruler-two-trait" type="two_trait" ' +
      'title="Make a Two Trait ruling with these rolls and the ones on the row below."><a href="#">2T</a></li>' +
      '<li class="dice-ruler-button dice-ruler-a dice-ruler-two-trait-vs" type="two_trait_vs" ' +
      'title="Make a Two Trait Versus ruling with these rolls and the ones on the row below."><a href="#">2Tv</a></li>' +
      '<li class="dice-ruler-button dice-ruler-b dice-ruler-two-trait-vs" type="two_trait_vs_b" ' +
      'title="Make a Two Trait Versus ruling with these rolls and the ones on the row below. Second Party."><a href="#">2Tv</a></li>' +
      '</ul>';
    $('.dice_ruled .dice').prepend(rule_buttons);
    // Second set of buttons.
    var rule_buttons = '<ul class="dice_rule dice-ruler-buttons dice-ruler-set-buttons"> ' +
      '<li class="dice-ruler-button dice-ruler-a dice-ruler-combat" type="combat" ' +
      'title="Make a Combat ruling with all available Str, Dex and End rolls."><a href="#">C</a></li>' +
      '<li class="dice-ruler-button dice-ruler-a dice-ruler-magic" type="magic" ' +
      'title="Make a Magic ruling with all available Int and Spi rolls."><a href="#">M</a></li>' +
      '<li class="dice-ruler-button dice-ruler-a dice-ruler-tech" type="tech" ' +
      'title="Make a Tech ruling with all available Int and Dex rolls."><a href="#">T</a></li>' +
      '<li class="dice-ruler-button dice-ruler-b dice-ruler-combat-two" type="combat_b" ' +
      'title="Make a Combat ruling with all available Str, Dex and End rolls. Second Party."><a href="#">C</a></li>' +
       '</ul>';
    $('.dice_ruled .dice_sets').after(rule_buttons);
    // Add reset buttons to the ruling form.
    var reset = $('.dice-ruler-reset').length;
    if (reset == 0) {
      // Number the rows.
      // Primary reset.
      var reset_button = '<input type="button" class="form-submit dice-ruler-reset" value="Reset All">';
      $('#edit-rule-dice').after(reset_button);
      $('.dice-ruler-reset').hide();
      // Secondary reset.
      var row_count = 1;
      $('.form-type-select select[name*="roll_type"]').each(function(){
         reset_button = '<input type="button" class="form-submit dice-ruler-row-reset" '
           + 'value="Reset" row-id="' + row_count + '">';
         $(this).after(reset_button);
         row_count++;
       });
    }
  }

  // Clear the ruler as needed.
  function clearRuler() {
    last_slot = 0;
    vs_slot = 0;
    // For each of the four slots, update the view.
    for (slot = 1; slot < 5; slot++) {
      clearRow(slot);
    }
    // Change the dropdown.
    if ($('#dice-ruler-form select[name="actions"]').val() != 'hidden') {
      $('#dice-ruler-form select[name="actions"]').val('hidden').trigger('change');
    }
  }

  // Clear the ruler as needed.
  function clearRow(row_number) {
    var this_row = $('#dice-ruler-form #edit-action-' + row_number);
    // Clear the text values.
    $(this_row).find('input[type="text"]').val('');
    $(this_row).find('input[name*="number_actions_b"]').val(1);
    // Set the diff default.
    $(this_row).find('input[name*="two_trait_diff"]').val(25);
    $(this_row).find('input[name*="magic_diff"]').val(25);
    // Change the dropdown.
    $(this_row).find('.form-type-select select[name*="roll_type"]').val('two_trait').trigger('change');
  }

  // Clicking a row button.
  function clickRowButton(button) {
    // If the row we're updating is the newest one.
    if (last_slot == 0) {
      clearRuler();
    }
    // Get setup.
    var rolls = $(button).parents('.dice_rolls');
    var new_slot = last_slot;
    var button_type = $(button).parent('.dice-ruler-button').attr('type');
    var this_row = $(button).parents('.dice').attr('dice_row');
    this_row = parseInt(this_row);
    // Process the button.
    switch(button_type) {
      case 'two_trait':
        // Get the second trait from the next row.
        next_row = this_row + 1;
        $(rolls).find('.dice_row_' + this_row).each(function(index, value) {
          new_slot = new_slot + 1;
          var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
          $(new_form).find('input[name*="two_trait_might_a"]').val( $(this).attr('might') );
          $(new_form).find('input[name*="two_trait_rolled_a"]').val( $(this).attr('roll') );
          var this_set = $(this).parent('.dice_set').attr('set');
          $(rolls).find('.dice_row_' + next_row).each(function() {
            $(new_form).find('input[name*="two_trait_might_b"]').val( $(this).attr('might') );
            $(new_form).find('input[name*="two_trait_rolled_b"]').val( $(this).attr('roll') );
          });
        });
      break;
      case 'two_trait_vs':
        // Get the second trait from the next row.
        next_row = this_row + 1;
        $(rolls).find('.dice_row_' + this_row).each(function(index, value) {
          new_slot = new_slot + 1;
          var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
          $(new_form).find('input[name*="two_trait_vs_might_aa"]').val( $(this).attr('might') );
          $(new_form).find('input[name*="two_trait_vs_rolled_aa"]').val( $(this).attr('roll') );
          var this_set = $(this).parent('.dice_set').attr('set');
          $(rolls).find('.dice_row_' + next_row).each(function() {
            $(new_form).find('input[name*="two_trait_vs_might_ab"]').val( $(this).attr('might') );
            $(new_form).find('input[name*="two_trait_vs_rolled_ab"]').val( $(this).attr('roll') );
          });
        });
      break;
    }
    // Update the rows.
    clickUpdateRow(new_slot, button_type);
  }

  // Clicking a row button.
  function clickRowButtonB(button) {
    // Get setup.
    var rolls = $(button).parents('.dice_rolls');
    var new_slot = last_slot;
    var button_type = $(button).parent('.dice-ruler-button').attr('type');
    var this_row = $(button).parents('.dice').attr('dice_row');
    this_row = parseInt(this_row);
    // Process the button.
    switch(button_type) {
      case 'two_trait_vs_b':
        // Get the second trait from the next row.
        next_row = this_row + 1;
        $(rolls).find('.dice_row_' + this_row).each(function(index, value) {
          var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
          $(new_form).find('input[name*="two_trait_vs_might_ba"]').val( $(this).attr('might') );
          $(new_form).find('input[name*="two_trait_vs_rolled_ba"]').val( $(this).attr('roll') );
          var this_set = $(this).parent('.dice_set').attr('set');
          $(rolls).find('.dice_row_' + next_row).each(function() {
            $(new_form).find('input[name*="two_trait_vs_might_bb"]').val( $(this).attr('might') );
            $(new_form).find('input[name*="two_trait_vs_rolled_bb"]').val( $(this).attr('roll') );
          });
        });
      break;
    }
  }

  // Clicking a set button.
  function clickSetButton(button) {
    // If the row we're updating is the newest one.
    if (last_slot == 0) {
      clearRuler();
    }
    // Get setup.
    var rolls = $(button).parents('.dice_rolls');
    var new_slot = last_slot;
    var button_type = $(button).parent('.dice-ruler-button').attr('type');
    // Process the button.
    switch(button_type) {
      case 'combat':
        // Get all str, dex, end combinations.
        $(rolls).find('.dice_sets').each(function() {
          if ($(this).find('.dice_roll_str').length > 0 && $(this).find('.dice_roll_dex').length > 0) {
            new_slot = new_slot + 1;
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find('input[name*="combat_might_a_dex"]').val( $(this).find('.dice_roll_dex').attr('might') );
            $(new_form).find('input[name*="combat_might_a_str"]').val( $(this).find('.dice_roll_str').attr('might') );
            $(new_form).find('input[name*="combat_might_a_end"]').val( $(this).find('.dice_roll_end').attr('might') );
            $(new_form).find('input[name*="combat_rolled_a_dex"]').val( $(this).find('.dice_roll_dex').attr('roll') );
            $(new_form).find('input[name*="combat_rolled_a_str"]').val( $(this).find('.dice_roll_str').attr('roll') );
            $(new_form).find('input[name*="combat_rolled_a_end"]').val( $(this).find('.dice_roll_end').attr('roll') );
          }
        });
      break;
      case 'magic':
        // Get all int, spi combinations.
        $(rolls).find('.dice_sets').each(function() {
          if ($(this).find('.dice_roll_int').length > 0 && $(this).find('.dice_roll_spi').length > 0) {
            new_slot = new_slot + 1;
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find('input[name*="magic_might_a"]').val( $(this).find('.dice_roll_int').attr('might') );
            $(new_form).find('input[name*="magic_might_b"]').val( $(this).find('.dice_roll_spi').attr('might') );
            $(new_form).find('input[name*="magic_rolled_a"]').val( $(this).find('.dice_roll_int').attr('roll') );
            $(new_form).find('input[name*="magic_rolled_b"]').val( $(this).find('.dice_roll_spi').attr('roll') );
          }
        });
      break;
      case 'tech':
        // Get all int, dex combinations.
        $(rolls).find('.dice_sets').each(function() {
          if ($(this).find('.dice_roll_int').length > 0 && $(this).find('.dice_roll_dex').length > 0) {
            new_slot = new_slot + 1;
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find('input[name*="two_trait_might_a"]').val( $(this).find('.dice_roll_int').attr('might') );
            $(new_form).find('input[name*="two_trait_might_b"]').val( $(this).find('.dice_roll_dex').attr('might') );
            $(new_form).find('input[name*="two_trait_rolled_a"]').val( $(this).find('.dice_roll_int').attr('roll') );
            $(new_form).find('input[name*="two_trait_rolled_b"]').val( $(this).find('.dice_roll_dex').attr('roll') );
          }
        });
        button_type = 'two_trait';
      break;
    }
    // Update the rows.
    clickUpdateRow(new_slot, button_type);
  }

  // Clicking a set button.
  function clickSetButtonB(button) {
    // Get setup.
    var rolls = $(button).parents('.dice_rolls');
    var new_slot = last_slot;
    var button_type = $(button).parent('.dice-ruler-button').attr('type');
    // Process the button.
    switch(button_type) {
      case 'combat_b':
        // Get all str, dex, end combinations.
        $(rolls).find('.dice_sets').each(function() {
          if ($(this).find('.dice_roll_str').length > 0 && $(this).find('.dice_roll_dex').length > 0) {
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find('input[name*="combat_might_b_dex"]').val( $(this).find('.dice_roll_dex').attr('might') );
            $(new_form).find('input[name*="combat_might_b_str"]').val( $(this).find('.dice_roll_str').attr('might') );
            $(new_form).find('input[name*="combat_might_b_end"]').val( $(this).find('.dice_roll_end').attr('might') );
            $(new_form).find('input[name*="combat_rolled_b_dex"]').val( $(this).find('.dice_roll_dex').attr('roll') );
            $(new_form).find('input[name*="combat_rolled_b_str"]').val( $(this).find('.dice_roll_str').attr('roll') );
            $(new_form).find('input[name*="combat_rolled_b_end"]').val( $(this).find('.dice_roll_end').attr('roll') );
          }
        });
      break;
    }
  }

  // Update the row settings.
  function clickUpdateRow(new_slot, button_type) {
    // Limit to 4 at most.
    if (new_slot > 3) {
      new_slot = 4;
      vs_slot = 4;
    }
    // Set the actions.
    $('#dice-ruler-form select[name="actions"]').val(new_slot).trigger('change');
    // We don't want to change any but the new ones, but set it to the asked for form type.
    for (var form_number = last_slot + 1; form_number <= new_slot; form_number++) {
      $('#dice-ruler-form #edit-action-' + form_number + ' select[name="roll_type_' + form_number + '"]').val(button_type).trigger('change');
      last_slot = last_slot + 1;
    }
    // Wrap over to 0.
    if (last_slot > 3) {
      last_slot = 0;
      vs_slot = 0;
    }
  }

  // Once, on page load, add the pause button to the interface.
  $('document').ready(function() {
    // Set up the form for us, but only once.
    if ($('.dice_rule').length == 0) {
      setupRuler();
      // Show or hide the reset button based on the actions selected.
      $('#dice-ruler-form select[name="actions"]').change(function() {
        if ($(this).val() == 'hidden') {
          clearRuler();
          $('.dice-ruler-reset').hide();
        }
        else {
          $('.dice-ruler-reset').show();
        }
      });
      // Add click listener for the ruler.
      $('.dice-ruler-reset').click(function() {
        clearRuler();
        return false;
      });
      // Add the refresh buttons to all rows.
      $('.dice-ruler-row-reset').click(function() {
        var row_number = $(this).attr('row-id');
        clearRow(row_number);
        return false;
      });
      // Make the buttons give feedback if hovered.
      $('.dice-ruler-buttons li.dice-ruler-a').mouseover(function() {
        $(this).css('border-color', '#0A0');
      });
      $('.dice-ruler-buttons li.dice-ruler-b').mouseover(function() {
        $(this).css('border-color', '#0A0');
      });
      $('.dice-ruler-buttons li.dice-ruler-a').mouseout(function() {
        $(this).css('border-color', '#EEE');
      });
      $('.dice-ruler-buttons li.dice-ruler-b').mouseout(function() {
        $(this).css('border-color', '#A00');
      });
      // Load up just this row when clicked on.
      $('.dice-ruler-row-buttons .dice-ruler-a a').click(function() {
        clickRowButton(this);
        return false;
      });
      // Vs only for the two vs settings.
      $('.dice-ruler-row-buttons .dice-ruler-b a').click(function() {
        clickRowButtonB(this);
        return false;
      });
      // Load it all up when clicked on.
      $('.dice-ruler-set-buttons .dice-ruler-a a').click(function() {
        clickSetButton(this);
        return false;
      });
      // Vs only for the combat vs settings.
      $('.dice-ruler-set-buttons .dice-ruler-b a').click(function() {
        clickSetButtonB(this);
        return false;
      });
    }
  });

  })(jQuery); }
}
