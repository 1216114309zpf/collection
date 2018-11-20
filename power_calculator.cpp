class Solution {
public:
    enum op{PLUS, MINUS, TIME, DIVIDE, POWER, PARENT_LEFT, PARENT_RIGHT};
    int calculate(string s) {
        vector<string> tokens = parse(s);
        reverse(tokens.begin(), tokens.end());
        stack<op> opstk;
        stack<int> intstk;
        for(int i = 0; i < tokens.size(); i++){
            if(tokens[i] == ")"){
                opstk.push(PARENT_LEFT);
            }else if(tokens[i] == "("){
                while(opstk.top() != PARENT_LEFT){
                    calculateOnce(intstk, opstk);
                }
                opstk.pop();
            }else if(tokens[i] == "^"){
                opstk.push(POWER);
            }else if(tokens[i] == "*" or tokens[i] == "/"){
                while(!opstk.empty() and opstk.top() == POWER){
                    calculateOnce(intstk, opstk);
                }
                opstk.push(tokens[i] == "*"?TIME:DIVIDE);
            }else if(tokens[i] == "+" or tokens[i] == "-"){
                while(!opstk.empty() and opstk.top() != PLUS and opstk.top() != MINUS and opstk.top() != PARENT_LEFT){
                    calculateOnce(intstk, opstk);
                }
                opstk.push(tokens[i] == "+"?PLUS:MINUS);
            }else{ //number
                intstk.push(str2int(tokens[i]));
            }
        }
        while(!opstk.empty()){
            calculateOnce(intstk, opstk);
        }
        cout<<intstk.top()<<endl;
        return intstk.top();
    }
    
    int str2int(string str){
        int result = 0;
        for(int i = 0; i < str.size(); i++){
            result *= 10;
            result += (str[i] - '0');
        }
        return result;
    }
    
    vector<string> parse(string str){
        vector<string> result;
        int itr = 0;
        while(itr < str.size()){
            if(str[itr] == ' '){
                itr++;
                continue;
            }else if(str[itr] >= '0' and str[itr]<= '9'){
                string tmp;
                while(itr < str.size() and str[itr] >= '0' and str[itr] <= '9'){
                    tmp.push_back(str[itr]);
                    ++itr;
                }
                result.push_back(tmp);
                --itr;
            }else{
                string tmp = str.substr(itr, 1);
                result.push_back(tmp);
            }
            ++itr;
        }
        return result;
    }
    
    void calculateOnce(stack<int>& intstk, stack<op>& opstk){
        op opt = opstk.top();
        opstk.pop();
        int first = intstk.top();
        intstk.pop();
        int second = intstk.top();
        intstk.pop();
        int result = getCalculate(first, second, opt);
        intstk.push(result);
    }
    
    int getCalculate(int first, int second, op opt){
        if(opt == PLUS){
            return first + second;
        }else if(opt == MINUS){
            return first - second;
        }else if(opt == TIME){
            return first * second;
        }else if(opt == DIVIDE){
            return first / second;
        }else if(opt == POWER){
            return pow(first,second);
        }
    }
};
