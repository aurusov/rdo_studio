function swapSections( invisibleNum, visibleNum ) {
	if ( invisibleNum.style.display == "none" ) {
		invisibleNum.style.display = "";
		visibleNum.style.display = "none";
	}
}