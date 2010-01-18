// $Id: img_assist.js,v 1.5 2009/06/13 01:17:21 sun Exp $

Drupal.wysiwyg.plugins.imp_imgupload = {

  /**
   * Return whether the passed node belongs to this plugin.
   */
  isNode: function (node) {
    return $(node).is('img.imp_imgupload');
  },

  /**
   * Execute the button.
   */
  invoke: function (data, settings, instanceId) {
    if (data.format == 'html') {
      // captionTitle and captionDesc for backwards compatibility.
      var options = {nid: '', title: '', captionTitle: '', desc: '', captionDesc: '', link: '', url: '', align: '', width: '', height: '', id: instanceId, action: 'insert'};
      if ($(data.node).is('img.img-assist')) {
        options.align = data.node.align;
        // Expand inline tag in alt attribute
        data.node.alt = decodeURIComponent(data.node.alt);
		data.node.title = decodeURIComponent(data.node.title);
/*        var chunks = data.node.alt.split('|');
        for (var i in chunks) {
          chunks[i].replace(/([^=]+)=(.*)/g, function(o, property, value) {
            options[property] = value;
          });
        }*/
        options.captionTitle = options.title;
        options.captionDesc = options.desc;
        options.action = 'update';
      }
    }
    else {
      // @todo Plain text support.
    }
    if (typeof options != 'undefined') {
	  form_id =  $("form[id='node-form'] input[name='form_build_id']").val();	
    var aurl = '/index.php?q=ajax/imp_imgupload/load/'+form_id+'&popup=1';
	  btns = {
	  		"Einfügen": function(){
				// well lets test if an image has been selected
		  		if ($('#modalframe-element').contents().find('#uploadedImage').size() === 0) {
		  			alert("Please select an image to upload first");
		  			return;
		  		}
		  		// else
				imgUrl = $('#modalframe-element').contents().find('#uploadedImage').attr('src');
				imgTitle = $('#modalframe-element').contents().find('#uploadedImage').attr('title');
				imgAlign = $('#modalframe-element').contents().find('#edit-alignment :selected').val();
				var args = {
					url: imgUrl,
					title: imgTitle,
					align: imgAlign,
					success: true,
					form_id: form_id,
					editor_id : instanceId
				}								
				Drupal.wysiwyg.plugins.imp_imgupload.createImageInContent(args);
				Drupal.modalFrame.close();
			}
	  };
			  
	  btns.Abbrechen =  function() {	  	
	  	Drupal.wysiwyg.plugins.imp_imgupload.updateAttachmentTable ({form_id: form_id}, 'Canceled'); 
	  	$(this).dialog("close"); 
		} ;
	  Drupal.modalFrame.open({ url : aurl,autoFit:true, autoResize:true, draggable : false, width:500, height:200,buttons: btns, onSubmit:Drupal.wysiwyg.plugins.imp_imgupload.updateAttachmentTable});
    }
  },
  
  createImageInContent: function(args) {
		var ed = tinyMCE.get(args.editor_id);		
		img = "<img class='imp_imgupload' src='"+args.url+"' title='"+args.title+"' alt='Bild:"+args.title+"' align='"+args.align+"'>";		
		ed.execCommand("mceBeginUndoLevel");    
		ed.execCommand("mceInsertContent", false, img);
		ed.execCommand("mceEndUndoLevel");
  },
  /*
   * regenerating the attachment table of the main form
   */
  updateAttachmentTable: function(args, message) {
  	form_id =  $("form[id='node-form'] input[name='form_build_id']").val();	
  	var aurl = '/ajax/imp_imgupload/rebuildAttchedTable/'+form_id;
  	$.get(aurl,null,function(data,status) {
		// we have no table yet, so lets create it
		if ($('#file-attachments').size() === 0) {
			$('#file-attach-wrapper').prepend(data);			
		}	
	 	else{
			// the table exist, we replace it completely
			// as we generated the whole table, not just the part to append
			$('#file-attachments').replaceWith(data);
		}		
	});
  },
  
  
  /**
   * Replace inline tags in content with images.
   */
  attach: function (content, settings, instanceId) {
    content = content.replace(/\[img_assist\|([^\[\]]+)\]/g, function(orig, match) {
      var node = {}, chunks = match.split('|');
      for (var i in chunks) {
        chunks[i].replace(/([^=]+)=(.*)/g, function(o, property, value) {
          node[property] = value;
        });
      }
      // 'class' is a predefined token in JavaScript.
      node['class'] = 'img-assist drupal-content';
      node.src = Drupal.settings.basePath + 'index.php?q=image/view/' + node.nid;
      node.alt = 'nid=' + node.nid + '|title=' + node.title + '|desc=' + node.desc;
      if (node.link.indexOf(',') != -1) {
        var link = node.link.split(',', 2);
        node.alt += '|link=' + link[0] + '|url=' + link[1];
      }
      else {
        node.alt += '|link=' + node.link;
      }
      if (typeof node.url != 'undefined') {
        node.alt += '|url=' + node.url;
      }
      node.alt = encodeURIComponent(node.alt);
      var element = '<img ';
      for (var property in node) {
        element += property + '="' + node[property] + '" ';
      }
      element += '/>';
      return element;
    });
    return content;
  },

  /**
   * Replace images with inline tags in content upon detaching editor.
   */
  detach: function (content, settings, instanceId) {
    var $content = $('<div>' + content + '</div>'); // No .outerHTML() in jQuery :(
    $('img.imp_imgupload', $content).each(function(node) {
      var inlineTag = '[file:src='+this.src+'|nid='+this.nid+'|' + decodeURIComponent(this.alt) + '|align=' + this.align + '|width=' + this.width + '|height=' + this.height + ']';
      $(this).replaceWith(inlineTag);
    });
    return $content.html();
  }
};
