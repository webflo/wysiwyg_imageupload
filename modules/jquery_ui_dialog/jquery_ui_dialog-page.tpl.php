<?php
// $Id: modalframe-page.tpl.php,v 1.1.2.4 2009/06/19 16:20:04 markuspetrux Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
<?php print $head; ?>
<title><?php print (!empty($title) ? strip_tags($title) : $head_title); ?></title>
<?php print $styles; ?>
<?php print $scripts; ?>
</head>
<body>
<div class="dialog-page-wrapper">
  <div class="dialog-page-container clear-block">
    <div class="dialog-page-content">
<?php if ($show_messages && $messages): print $messages; endif; ?>
<?php print $help; ?>
<div class="clear-block">
  <?php print $content; ?>
</div>
    </div>
  </div>
</div>
<?php print $closure; ?>
</body>
</html>
