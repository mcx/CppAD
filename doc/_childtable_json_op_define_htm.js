// Child table for section json_op_define
document.write('\
<select onchange="json_op_define_child(this)">\
<option>json_op_define-&gt;</option>\
<option>json_add_op.cpp</option>\
<option>json_atom_op.cpp</option>\
<option>json_div_op.cpp</option>\
<option>json_mul_op.cpp</option>\
<option>json_sub_op.cpp</option>\
<option>json_sum_op.cpp</option>\
</select>\
');
function json_op_define_child(item)
{	var child_list = [
		'json_add_op.cpp.htm',
		'json_atom_op.cpp.htm',
		'json_div_op.cpp.htm',
		'json_mul_op.cpp.htm',
		'json_sub_op.cpp.htm',
		'json_sum_op.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
