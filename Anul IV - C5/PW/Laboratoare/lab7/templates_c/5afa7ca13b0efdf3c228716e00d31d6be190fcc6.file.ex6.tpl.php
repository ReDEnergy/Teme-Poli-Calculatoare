<?php /* Smarty version Smarty-3.1.17, created on 2014-04-04 11:28:10
         compiled from "ex6.tpl" */ ?>
<?php /*%%SmartyHeaderCode:25981533e79bb022368-96975000%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '5afa7ca13b0efdf3c228716e00d31d6be190fcc6' => 
    array (
      0 => 'ex6.tpl',
      1 => 1396603687,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '25981533e79bb022368-96975000',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1.17',
  'unifunc' => 'content_533e79bb49eb66_43627144',
  'variables' => 
  array (
    'name' => 0,
    'age' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_533e79bb49eb66_43627144')) {function content_533e79bb49eb66_43627144($_smarty_tpl) {?><form id="form" method="POST" action="ex6.php">
	<label for="name"> Name </label>
	<input type="text" name="name" value="<?php echo htmlspecialchars($_smarty_tpl->tpl_vars['name']->value, ENT_QUOTES, 'UTF-8', true);?>
" size="40">
	<label for="age"> Age </label>
	<input type="text" name="age" value="<?php echo htmlspecialchars($_smarty_tpl->tpl_vars['age']->value, ENT_QUOTES, 'UTF-8', true);?>
" size="40">
	<input type="submit" value="Submit">
</form><?php }} ?>
