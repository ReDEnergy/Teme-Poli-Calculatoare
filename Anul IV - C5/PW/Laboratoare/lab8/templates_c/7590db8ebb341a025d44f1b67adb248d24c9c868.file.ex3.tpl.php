<?php /* Smarty version Smarty-3.1.17, created on 2014-04-04 09:57:01
         compiled from "ex3.tpl" */ ?>
<?php /*%%SmartyHeaderCode:5300533e62b25b8a74-72404349%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '7590db8ebb341a025d44f1b67adb248d24c9c868' => 
    array (
      0 => 'ex3.tpl',
      1 => 1396598204,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '5300533e62b25b8a74-72404349',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.17',
  'unifunc' => 'content_533e62b26742a2_57399483',
  'variables' => 
  array (
    'persons' => 0,
    'person' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_533e62b26742a2_57399483')) {function content_533e62b26742a2_57399483($_smarty_tpl) {?><table>
	<?php echo $_smarty_tpl->getSubTemplate ('header.tpl', $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, null, array(), 0);?>

<?php  $_smarty_tpl->tpl_vars['person'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['person']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['persons']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['person']->key => $_smarty_tpl->tpl_vars['person']->value) {
$_smarty_tpl->tpl_vars['person']->_loop = true;
?>
	<?php echo $_smarty_tpl->getSubTemplate ('entry.tpl', $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, null, array('person'=>$_smarty_tpl->tpl_vars['person']->value), 0);?>

<?php } ?>
<table><?php }} ?>
