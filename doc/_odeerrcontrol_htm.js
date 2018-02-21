var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'utility.htm',
'odeerrcontrol.htm'
];
var list_down2 = [
'install.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down1 = [
'errorhandler.htm',
'nearequal.htm',
'speed_test.htm',
'speedtest.htm',
'time_test.htm',
'test_boolofvoid.htm',
'numerictype.htm',
'checknumerictype.htm',
'simplevector.htm',
'checksimplevector.htm',
'nan.htm',
'pow_int.htm',
'poly.htm',
'ludetandsolve.htm',
'rombergone.htm',
'rombergmul.htm',
'runge45.htm',
'rosen34.htm',
'odeerrcontrol.htm',
'odegear.htm',
'odegearcontrol.htm',
'cppad_vector.htm',
'thread_alloc.htm',
'index_sort.htm',
'to_string.htm',
'set_union.htm',
'sparse_rc.htm',
'sparse_rcv.htm',
'sparse2eigen.htm'
];
var list_down0 = [
'ode_err_control.cpp.htm',
'ode_err_maxabs.cpp.htm'
];
var list_current0 = [
'odeerrcontrol.htm#Syntax',
'odeerrcontrol.htm#Description',
'odeerrcontrol.htm#Include',
'odeerrcontrol.htm#Notation',
'odeerrcontrol.htm#xf',
'odeerrcontrol.htm#Method',
'odeerrcontrol.htm#Method.step',
'odeerrcontrol.htm#Method.Nan',
'odeerrcontrol.htm#Method.order',
'odeerrcontrol.htm#ti',
'odeerrcontrol.htm#tf',
'odeerrcontrol.htm#xi',
'odeerrcontrol.htm#smin',
'odeerrcontrol.htm#smax',
'odeerrcontrol.htm#scur',
'odeerrcontrol.htm#eabs',
'odeerrcontrol.htm#erel',
'odeerrcontrol.htm#ef',
'odeerrcontrol.htm#maxabs',
'odeerrcontrol.htm#nstep',
'odeerrcontrol.htm#Error Criteria Discussion',
'odeerrcontrol.htm#Scalar',
'odeerrcontrol.htm#Vector',
'odeerrcontrol.htm#Example',
'odeerrcontrol.htm#Theory',
'odeerrcontrol.htm#Source Code'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
