<table>
	{include file='header.tpl'}
	{foreach item=person from=$persons}
		{include file='entry.tpl' person=$person}
	{/foreach}
<table>

<div>
	{for $p=0 to $pages}
		{if $page == $p}
			<strong>
		{/if}
		<a href="ex8.php?p={$p}">{$p}</a>	
		{if $page == $p}
			</strong>
		{/if}
	{/for}
</div>