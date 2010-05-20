// $Id:$
// Helper method.
(function($) {
  $.fn.imguploadOuterHTML = function (s) {
    return (s) ? this.before(s).remove() : jQuery("<p>").append(this.eq(0).clone()).html();
  };

  Drupal.wysiwyg.plugins.imgupload = {
    /**
    * Return whether the passed node belongs to this plugin.
    */
    isNode: function (node) {
      $node = this.getRepresentitiveNode(node);
      return $node.is('img.imgupload');
    },

    /* We need this due all the special cases in the editors */
    getRepresentitiveNode: function(node) {
      if(node.$) {
        // This case is for the CKeditor, where
        // $(node) != $(node.$)
        return $(node.$);
      }
      // else
      // This would be for the TinyMCE and hopefully others
      return $(node)
    },

    /**
    * Execute the button.
    */
    invoke: function (data, settings, instanceId) {
      if (data.format == 'html') {
        // Default
        var options = {
          id: instanceId,
          action: 'insert'
        };
        var $node = null;
        if (data.node) {
          $node = this.getRepresentitiveNode(data.node);
        }

        if ($node != null && $node.is('img') && $node.hasClass('imgupload')) {
          $n = $(data.node);
          options.iid = decodeURIComponent(data.node.getAttribute('alt'));
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
      // Create the buttons
      dialogIframe = Drupal.jqui_dialog.iframeSelector();
      btns = {};
      btns[Drupal.t('Insert')] = function () {
        // well lets test if an image has been selected
        if ($(dialogIframe).contents().find('form#wysiwyg-imageupload-upload-form').find('input#edit-wysiwyg-imageupload-file').size() > 0) {
          alert(Drupal.t("Please select an image to upload first"));
          return;
        }
        // else
        var iid = 0;
        $(dialogIframe).contents().find('form#wysiwyg-imageupload-upload-form').ajaxSubmit({
          dataType : 'json',
          method: 'post',
          async: false,
          success : function(data,status,xhr,jq) {
              iid = data.data.iid;
              if(!iid) {
                return;
              }
            }
          }
        );
        Drupal.wysiwyg.plugins.imgupload.createImageInContent(iid,instanceId);
        $(this).dialog("close");
      };

      btns[Drupal.t('Cancel')] = function () {
        $(this).dialog("close");
      };
      var form_id = Drupal.settings.wysiwyg_imageupload.current_form;
      // Location, where to fetch the dialog.
      var aurl = Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/add/' + form_id;
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
      // Create buttons.
      dialogIframe = Drupal.jqui_dialog.iframeSelector();
      btns = {};
      // Update button.
      btns[Drupal.t('Update')] = function () {
        var iid = 0;
        $(dialogIframe).contents().find('form#wysiwyg-imageupload-edit-form').ajaxSubmit({
          dataType : 'json',
          method: 'post',
          async: false,
          success : function(data,status,xhr,jq) {
              iid = data.data.iid;
              if(!iid) {
                return;
              }
            }
          }
        );
        Drupal.wysiwyg.plugins.imgupload.createImageInContent(iid,instanceId);
        $(this).dialog("close");
      };
      // Cancel button
      btns[Drupal.t('Cancel')] = function () {
        $(this).dialog("close");
      };

      // Location, where to fetch the dialog.
      var aurl = Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/edit/' + options.iid;
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
    createImageInContent: function (iid,editor_id) {
      var aurl = Drupal.settings.basePath + 'index.php?q=ajax/wysiwyg_imgupl/render_wysiwyg/' + iid;

      $.get(
          aurl,
          null,
          function (data, status) {
            img = $(data.data).imguploadOuterHTML();
            Drupal.wysiwyg.plugins.imgupload.insertIntoEditor(img,editor_id);
          },
          'json'
      );
    },

    /*
    * Thats the most critical part. Call the WYSIWYG API to insert this html into
    * the current editor, no matter what editor it might be
    */
    insertIntoEditor: function (data, editor_id) {
      // This is all the magic
      Drupal.wysiwyg.instances[editor_id].insert(data);
    },
  };
})(jQuery);