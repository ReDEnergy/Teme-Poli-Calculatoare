<?php /* Smarty version Smarty-3.1.17, created on 2014-04-04 11:57:54
         compiled from "ex8.tpl" */ ?>
<?php /*%%SmartyHeaderCode:11389533e8037cebee5-37962709%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '54536fb3e55b5fb244c947f1a0af136f1951947b' => 
    array (
      0 => 'ex8.tpl',
      1 => 1396605472,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '11389533e8037cebee5-37962709',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.17',
  'unifunc' => 'content_533e80380fecf7_24245251',
  'variables' => 
  array (
    'persons' => 0,
    'person' => 0,
    'pages' => 0,
    'page' => 0,
    'p' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_533e80380fecf7_24245251')) {function content_533e80380fecf7_24245251($_smarty_tpl) {?><table>
	<?php echo $_smarty_tpl->getSubTemplate ('header.tpl', $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, null, array(), 0);?>

	<?php  $_smarty_tpl->tpl_vars['person'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['person']->_loop = false;
 $_from = $_smarty_tpl->tpl_vars['persons']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['person']->key => $_smarty_tpl->tpl_vars['person']->value) {
$_smarty_tpl->tpl_vars['person']->_loop = true;
?>
		<?php echo $_smarty_tpl->getSubTemplate ('entry.tpl', $_smarty_tpl->cache_id, $_smarty_tpl->compile_id, 0, null, array('person'=>$_smarty_tpl->tpl_vars['person']->value), 0);?>

	<?php } ?>
<table>

<div>
	<?php $_smarty_tpl->tpl_vars['p'] = new Smarty_Variable;$_smarty_tpl->tpl_vars['p']->step = 1;$_smarty_tpl->tpl_vars['p']->total = (int) ceil(($_smarty_tpl->tpl_vars['p']->step > 0 ? $_smarty_tpl->tpl_vars['pages']->value+1 - (0) : 0-($_smarty_tpl->tpl_vars['pages']->value)+1)/abs($_smarty_tpl->tpl_vars['p']->step));
if ($_smarty_tpl->tpl_vars['p']->total > 0) {
for ($_smarty_tpl->tpl_vars['p']->value = 0, $_smarty_tpl->tpl_vars['p']->iteration = 1;$_smarty_tpl->tpl_vars['p']->iteration <= $_smarty_tpl->tpl_vars['p']->total;$_smarty_tpl->tpl_vars['p']->value += $_smarty_tpl->tpl_vars['p']->step, $_smarty_tpl->tpl_vars['p']->iteration++) {
$_smarty_tpl->tpl_vars['p']->first = $_smarty_tpl->tpl_vars['p']->iteration == 1;$_smarty_tpl->tpl_vars['p']->last = $_smarty_tpl->tpl_vars['p']->iteration == $_smarty_tpl->tpl_vars['p']->total;?>
		<?php if ($_smarty_tpl->tpl_vars['page']->value==$_smarty_tpl->tpl_vars['p']->value) {?>
			<strong>
		<?php }?>
		<a href="ex8.php?p=<?php echo $_smarty_tpl->tpl_vars['p']->value;?>
"><?php echo $_smarty_tpl->tpl_vars['p']->value;?>
</a>	
		<?php if ($_smarty_tpl->tpl_vars['page']->value==$_smarty_tpl->tpl_vars['p']->value) {?>
			</strong>
		<?php }?>
	<?php }} ?>
</div><?php }} ?>
