Drupal.behaviors.wysiwyg_imageupload_uploadform= function () {

  $('#edit-wysiwyg-imageupload-file').bind('change', function() {
      $('.dialog-page-content *').hide();
      $('.dialog-page-content').addClass('uploading');
      $('form').submit();
    }
  );
}