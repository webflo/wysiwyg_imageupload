// $Id: img_assist.js,v 1.5 2009/06/13 01:17:21 sun Exp $
// Helper method.
jQuery.fn.imguploadOuterHTML = function (s) {
  return (s) ? this.before(s).remove() : jQuery("<p>").append(this.eq(0).clone()).html();
};

Drupal.wysiwyg.plugins.imgupload = {
  /**
   * Return whether the passed node belongs to this plugin.
   */
  isNode: function (node) {
    //return $(node).is('img.imgupload');
    $node = $(node).find('img.imgupload')
    /* all img descendants of the root element */
    .andSelf()
    /* and the root element itself */
    .filter('img.imgupload');
    /* but if the root is not an img we don't need it */

    if ($node.length == 1) {
      return true;
    }
    //else 
    return false;
  },

  /**
   * Execute the button.
   */
  invoke: function (data, settings, instanceId) {
    if (data.format == 'html') {
      // Default
      var options = {
        title: '',
        src: '',
        align: '',
        width: '',
        height: '',
        id: instanceId,
        action: 'insert'
      };
      // Is a img selected in the content, which we can edit?
      // We can use is.(img.imgupload) here, as some editors pass this node surrounded by a <p>
      // tag
      $node = $(data.node).find('img.imgupload')
      /* all img descendants of the root element */
      .andSelf()
      /* and the root element itself */
      .filter('img.imgupload');
      /* but if the root is not an img we don't need it */

      if ($node.length == 1) {
        data.node = $node.get(0);
        options.floating = data.node.align;
        // Expand inline tag in alt attribute
        options.alt = decodeURIComponent(data.node.alt);
        options.title = decodeURIComponent(data.node.title);
        options.src = $(data.node).attr('src');
        options.classes = $(data.node).attr('class');
        options.action = 'update';
      }
    }
    else {
      // @todo Plain text support.
    }
    // Add or update.
    if (options.action == 'insert') {
      Drupal.wysiwyg.plugins.imgupload.add_form(data, settings, instanceId);
    }
    else if (options.action == 'update') {
      Drupal.wysiwyg.plugins.imgupload.update_form(data, settings, instanceId, options);
    }
  },

  /*
   * Open a dialog and present the add-image form.
   */
  add_form: function (data, settings, instanceId) {
    // We need the form ID to play with the cache of this form.
    form_id = $("form#node-form input[name='form_build_id']").val();

    // Location, where to fetch the dialog.
    var aurl = Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/add/' + form_id;

    // Create the buttons
    dialogIframe = Drupal.jqui_dialog.iframeSelector();
    btns = {};
    btns[Drupal.t('Insert')] = function () {
      // well lets test if an image has been selected
      if ($(dialogIframe).contents().find('#uploadedImage').size() === 0) {
        alert(Drupal.t("Please select an image to upload first"));
        return;
      }
      // else
      // Fetch all form-data settings
      var args = {
        title: $(dialogIframe).contents().find('#edit-title').val(),
        floating: $(dialogIframe).contents().find('#edit-alignment :selected').val(),
        style: $(dialogIframe).contents().find('#edit-style :selected').val(),
        preset: $(dialogIframe).contents().find('#edit-preset :selected').val(),
        cacheID: $(dialogIframe).contents().find('#uploadedImage').attr('alt'),
        form_id: form_id,
        success: true,
        editor_id: instanceId
      };
      Drupal.wysiwyg.plugins.imgupload.createImageInContent(args);
      $(this).dialog("close");
    };

    btns[Drupal.t('Cancel')] = function () {
      $(this).dialog("close");
    };

    // Open the dialog, load the form.
    Drupal.jqui_dialog.open({
      url: aurl,
      buttons: btns,
      width: 540
    });
  },

  /*
   * Open a image-details dialog, prefilled with the current settings of the
   * selected image. 
   */
  update_form: function (data, settings, instanceId, options) {
    // Fill in the values got from the <img> object. Mostly converting css classes.
    options = Drupal.wysiwyg.plugins.imgupload.expand_options(options);

    // Location, where to fetch the dialog.
    var aurl = Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/update/' + encodeURIComponent(options.title) + '/' + encodeURIComponent(options.preset) + '/' + encodeURIComponent(options.floating) + '/' + encodeURIComponent(options.style) + '&imagepath=' + encodeURIComponent(options.alt);

    // Create buttons.
    dialogIframe = Drupal.jqui_dialog.iframeSelector();
    btns = {};
    // Update button.
    btns[Drupal.t('Update')] = function () {
      // Fetch all form-data settings
      var args = {
        title: $(dialogIframe).contents().find('#edit-title').val(),
        floating: $(dialogIframe).contents().find('#edit-alignment :selected').val(),
        style: $(dialogIframe).contents().find('#edit-style :selected').val(),
        preset: $(dialogIframe).contents().find('#edit-preset :selected').val(),
        cacheID: $(dialogIframe).contents().find('#uploadedImage').attr('alt'),
        success: true,
        editor_id: instanceId
      };
      // Insert the image into the editor, replacing the old one
      Drupal.wysiwyg.plugins.imgupload.updateImageInContent(args);
      $(this).dialog("close");
    };
    // Cancel button    
    btns[Drupal.t('Cancel')] = function () {
      $(this).dialog("close");
    };

    // Finally open the dialog.
    Drupal.jqui_dialog.open({
      url: aurl,
      buttons: btns,
      width: 540
    });
  },

  /*
   * Fetches the imagecache preset representitive and insert it all th way down into the current editor
   */
  createImageInContent: function (args) {
    var aurl = Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/showimage/' + args['cacheID'] + '/' + encodeURI(args['preset']) + '/' + encodeURI(args.title);

    $.get(aurl, null, function (data, status) {
      // Use some jquery foo to set th title and align      
      img = $(data);
      /* the whole tree returned by template */
      img.find('img')
      /* all img descendants of the root element */
      .andSelf()
      /* and the root element itself */
      .filter('img')
      /* but if the root is not an img we don't need it */
      .addClass('imgupload').addClass(args.floating).addClass(args.style).attr('title', args.title);
      img = img.imguploadOuterHTML();
      Drupal.wysiwyg.plugins.imgupload.insertIntoEditor(img, args.editor_id);
    });
  },

  /*
   * Updates the selected image. Yet, we just plainly replace it. 
   */
  updateImageInContent: function (args) {
    Drupal.wysiwyg.plugins.imgupload.createImageInContent(args);
  },

  /*
   * Thats the most critical part. Call the WYSIWYG API to insert this html into
   * the current editor, no matter what editor it might be
   */
  insertIntoEditor: function (data, editor_id) {
    // This is all the magic
    Drupal.wysiwyg.instances[editor_id].insert(data);
  },
  /*
   * Expands the options using some regexp. This is needed because floating and style is 
   * Added as class and needs to be extracted as "option information"
   */
  expand_options: function (options) {
    $(options.classes.split(' ')).each(function () {
      match = this.match(/(imgupl_floating_.*)/i);
      if (match != null) {
        options.floating = match[1];
      }

      match = this.match(/(imgupl_styles_.*)/i);
      if (match != null) {
        options.style = match[1];
      }
      /* Hack to support the module image_caption */
      match = this.match(/(caption)/i);
      if (match != null) {
        options.style = match[1];
      }

      match = this.match(/imagecache[-](.*)/i);
      if (match != null) {
        options.preset = match[1];
      }
    });

    return options;
  }
};