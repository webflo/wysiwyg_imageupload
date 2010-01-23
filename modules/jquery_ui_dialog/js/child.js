// $Id: child.js,v 1.1.2.13 2010/01/01 20:31:01 markuspetrux Exp $

(function ($) {
Drupal.jqui_dialogChild = Drupal.jqui_dialogChild || {
  processed: false
};

/**
 * Child dialog behavior.
 */
Drupal.jqui_dialogChild.attach = function(context) {  
  var self = Drupal.jqui_dialogChild;
  var settings = Drupal.settings.jqui_dialogChild || {};

  // If we cannot reach the parent window, then we have nothing else todo here.
  if (!self.isObject(parent.Drupal) || !self.isObject(parent.Drupal.jqui_dialog)) {
    return;
  }

  // Shortcuts to parent objects.
  self.$pWindow = parent.jQuery(parent);
  self.pJQui_dialog = parent.Drupal.jqui_dialog;

  // Make sure this behavior is not processed more than once.
  if (!self.processed) {
    self.processed = true;

    self.pJQui_dialog.chilDocumentSize  = {width: $(window).width(), height: $('body').height()+40};
    self.pJQui_dialog.childLoaded(window);
  }
  else {  
    //Drupal.jqui_dialogChild.recheckSize();
  }
};

Drupal.jqui_dialogChild.recheckSize = function() {
  self.pJQui_dialog = parent.Drupal.jqui_dialog;
  // we need this timeout otherwise the DOM might not be loaded fully yet. This would result in wrong
  // calculation of width / height
  setTimeout(function() {
    self.pJQui_dialog.chilDocumentSize  = {width: $(window).width(), height: $('body').height()+40}; 
    self.pJQui_dialog.resize();
  },100);
    
  //}
};

$(document).ready(function() {
  $('body').ajaxComplete( Drupal.jqui_dialogChild.recheckSize );
});

/**
 * Check if the given variable is an object.
 */
Drupal.jqui_dialogChild.isObject = function(something) {
  return (something !== null && typeof something === 'object');
};

Drupal.behaviors.jqui_dialogChild = Drupal.jqui_dialogChild.attach;

})(jQuery);