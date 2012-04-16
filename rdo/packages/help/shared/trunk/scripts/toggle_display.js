function toggle_display(e)
{
    if (e.style.display == "none")
        e.style.display = "";
    else
        e.style.display = "none";
    return false;
}