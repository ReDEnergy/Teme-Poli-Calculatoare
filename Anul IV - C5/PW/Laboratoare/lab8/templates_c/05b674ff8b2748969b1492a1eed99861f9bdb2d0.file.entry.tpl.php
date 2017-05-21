<?php /* Smarty version Smarty-3.1.17, created on 2014-04-04 12:00:10
         compiled from "entry.tpl" */ ?>
<?php /*%%SmartyHeaderCode:29396533e7e4b355f25-38414254%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '05b674ff8b2748969b1492a1eed99861f9bdb2d0' => 
    array (
      0 => 'entry.tpl',
      1 => 1396605606,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '29396533e7e4b355f25-38414254',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.17',
  'unifunc' => 'content_533e7e4b500824_09559723',
  'variables' => 
  array (
    'person' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_533e7e4b500824_09559723')) {function content_533e7e4b500824_09559723($_smarty_tpl) {?><tr>
	<td><?php echo $_smarty_tpl->tpl_vars['person']->value['name'];?>
</td>
	<td><?php echo $_smarty_tpl->tpl_vars['person']->value['age'];?>
</td>
</tr><?php }} ?>
