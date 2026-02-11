#include <stack>
#include <string>
using namespace std;

/**
 * Helper function to check if an opening bracket matches a closing bracket
 * @param opening: The opening bracket character
 * @param closing: The closing bracket character
 * @return true if they form a matching pair, false otherwise
 */
bool isMatchingPair(char opening, char closing)
{
    // Check if opening and closing brackets match
    // Valid pairs: (), [], {}
    return (opening == '(' && closing == ')') || (opening == '{' && closing == '}') || (opening == '[' && closing == ']');
}

bool isOpening(char ch)
{
    return ch == '(' || ch == '{' || ch == '[';
}

bool isClosing(char ch)
{
    return ch == ')' || ch == '}' || ch == ']';
}

/**
 * Checks if brackets in an arithmetic expression are properly matched
 * @param expression: A string containing an arithmetic expression
 * @return true if all brackets are properly matched and balanced, false otherwise
 */
bool isValidExpression(const string &expression)
{
    stack<char> st;

    for (char ch : expression)
    {
        if (isOpening(ch))
        {
            st.push(ch);
        }
        else if (isClosing(ch))
        {
            if (st.empty() || !isMatchingPair(st.top(), ch))
                return false;
            st.pop();
        }
    }

    return st.empty();
}
