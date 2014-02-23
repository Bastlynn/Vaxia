/* jQuery driven Eclipse Behaviors. */
(function ($) {
  Drupal.behaviors.themePullouts = {
    attach: function (context, settings) {


  // Add toggle buttons.
  $('.tabs').once(function() {
    // Setup.
    $('.sidebar-first #content').css('margin-left', '0%');
    $('.sidebar-first #content').css('width', '100%');
    $('.sidebar-first .region-sidebar-first').css('width', '100%');
    $('#header').hide();

    // Pullout for Header.
    $('#navigation').wrap('<div id="extruderBarHead" class="{title:\'View Header\'}"></div>');
    $('#extruderBarHead').buildMbExtruder({
      position:"left",
      width:"100%",
      positionFixed:false,
      top:60,
      extruderOpacity:0.8,
      onExtOpen:function(){
        $('#extruderBarHead').toggleClass('isOpen');
        $('#header').show();
        $('.sidebar-first #content').css('margin-left', '20%');
        $('.sidebar-first #content').css('width', '80%');
      },
      onExtContentLoad:function(){},
      onExtClose:function(){
        $('#extruderBarHead').toggleClass('isOpen');
        $('#header').hide();
        $('.sidebar-first #content').css('margin-left', '20%');
        $('.sidebar-first #content').css('width', '80%');
      }
    });

    // Pullout for Sidebar.
    $('.sidebar-first .region-sidebar-first').wrap('<div id="extruderBarFirst" class="{title:\'View Sidebar\'}"></div>');
    $('#extruderBarFirst').buildMbExtruder({
      position:"left",
      width:"100%",
      positionFixed:false,
      top:170,
      extruderOpacity:0.8,
      onExtOpen:function(){
        $('#extruderBarFirst').toggleClass('isOpen');
        $('.sidebar-first #content').css('margin-left', '20%');
        $('.sidebar-first #content').css('width', '80%');
      },
      onExtContentLoad:function(){},
      onExtClose:function(){
        $('#extruderBarFirst').toggleClass('isOpen');
        $('.sidebar-first #content').css('margin-left', '0%');
        $('.sidebar-first #content').css('width', '100%');
      }
    });

    // Pullout for Description.
    $('article.node-rpg-chatroom .field-name-field-wiki-source').wrap('<div id="extruderBarDesc" class="{title:\'View Description\'}"></div>');
    $('#extruderBarDesc').buildMbExtruder({
      position:"left",
      width:"100%",
      positionFixed:false,
      top:280,
      extruderOpacity:0.8,
      onExtOpen:function(){
        $('#extruderBarDesc').toggleClass('isOpen');
      },
      onExtContentLoad:function(){},
      onExtClose:function(){
        $('#extruderBarDesc').toggleClass('isOpen');
      }
    });

    // Pullout for Navigation.
    $('article.node-rpg-chatroom #node_rpg_chatroom_full_group_navigation').wrap('<div id="extruderBarNav" class="{title:\'View Navigation\'}"></div>');
    $('#extruderBarNav').buildMbExtruder({
      position:"left",
      width:"100%",
      positionFixed:false,
      top:402,
      extruderOpacity:0.8,
      onExtOpen:function(){
        $('#extruderBarNav').toggleClass('isOpen');
        $('.sidebar-first #content').css('margin-left', '20%');
        $('.sidebar-first #content').css('width', '80%');
      },
      onExtContentLoad:function(){},
      onExtClose:function(){
        $('#extruderBarNav').toggleClass('isOpen');
        $('.sidebar-first #content').css('margin-left', '0%');
        $('.sidebar-first #content').css('width', '100%');
      }
    });

  });

    }
  };
})(jQuery);