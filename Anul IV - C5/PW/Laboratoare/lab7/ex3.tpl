<table>
	{include file='header.tpl'}
{foreach item=person from=$persons}
	{include file='entry.tpl' person=$person}
{/foreach}
<table>