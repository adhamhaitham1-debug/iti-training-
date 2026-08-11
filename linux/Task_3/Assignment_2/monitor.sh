show_date() {
    echo "Current Date & Time: $(date)"
}
show_uptime() {
    echo "System Uptime: $(uptime -p)"
}
show_users() {
    echo "Logged-in Users:"
    users 2>/dev/null || who || echo "$USER"
}
show_pwd() {
    echo "Current Working Directory: $(pwd)"
}
show_disk() {
    echo "Disk Usage Summary:"
    df -h
}
show_memory() {
    echo "Memory Usage Summary:"
    free -h
}
list_files() {
    echo "Files in Current Directory:"
    ls -la
}
while true; do
echo ""
echo "====================="
echo "Linux Toolkit"
echo "====================="
echo "1) Show Date"
echo "2) Show Uptime"
    echo "3) Show Logged Users"
    echo "4) Show Current Directory"
    echo "5) Show Disk Usage"
    echo "6) Show Memory Usage"
    echo "7) List Files"
    echo "8) Exit"
    echo "====================="
    read -p "Enter choice [1-8]: " choice
case $choice in
1) show_date ;;
2) show_uptime ;;
3) show_users ;;
4) show_pwd ;;
5) show_disk ;;
6) show_memory ;;
7) list_files ;;
8) echo "Exiting... Goodbye!"; exit 0 ;;
*) echo "Invalid option. Please enter a number between 1 and 8." ;;
esac
done
