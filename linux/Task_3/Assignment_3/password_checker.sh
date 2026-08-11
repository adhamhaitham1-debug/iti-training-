read -p "Enter password: " pass
len_status="Missing"
upper_status="Missing"
lower_status="Missing"
digit_status="Missing"
special_status="Missing"
if [ ${#pass} -ge 8 ]; then
    len_status="OK"
fi
if [[ "$pass" =~ [A-Z] ]]; then
    upper_status="OK"
fi
if [[ "$pass" =~ [a-z] ]]; then
    lower_status="OK"
fi
if [[ "$pass" =~ [0-9] ]]; then
    digit_status="OK"
fi
if [[ "$pass" =~ [^a-zA-Z0-9] ]]; then
    special_status="OK"
fi
if [[ "$len_status" == "OK" && "$upper_status" == "OK" && "$lower_status" == "OK" && "$digit_status" == "OK" && "$special_status" == "OK" ]]; then
    overall="Strong"
else
    overall="Weak"
fi
echo ""
echo "Password Strength Report"
echo ""
echo "Length          : $len_status"
echo "Uppercase       : $upper_status"
echo "Lowercase       : $lower_status"
echo "Digit           : $digit_status"
echo "Special Char    : $special_status"
echo ""
echo "Overall: $overall"
