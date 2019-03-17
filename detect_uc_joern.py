#Code by Julien BATAILLE for an industral project of Telecom Nancy (NIST - SAMATE)

from joern.all import JoernSteps
import json
import unicodedata

j = JoernSteps()

j.setGraphDbURL('http://localhost:7474/db/data/')


j.connectToDatabase()

#get the array containing all the data of a node
def get_opt(node):
    opt = (node.split('{'))[1]
    opt = (opt.split('}'))[0]
    opts = []
    code=""
    c=False
    for i in opt.split(','):
        if i.find("code:")!=-1:
            code=i
            c=True
        elif c:
            if i.find("functionId:")!=-1:
                c=False
                opts.append(code)
                code=""
                opts.append(i)
            else:
                code=code+","+i

        else:
            opts.append(i)
    return opts

#get the id of a node
def get_id(s):
    s1 = (s.split('{'))[0][2:-1]
    return s1

#get the id of a function with it name
def get_fun_id(fun):
    res = j.runGremlinQuery("g.V().has('type','Function').has('name','"+fun+"')")
    for r in res:
        #s=str(r)
        #s1 = (s.split('{'))[0][2:-1]
        return get_id(str(r))
    return "0"

#get the functions that are not analyzed yet
def get_reste_fun(tab,funid):
    t = []
    for i in tab:
        if int(i[1])!=int(funid):
            t.append(i)
    return t

#get the sons of a node
def get_sons(id):
    res = j.runGremlinQuery("g.v("+str(id)+").out()")
    ret = []
    for r in res:
        ret.append(str(r))
    return ret

#get the number of the argument of a function, example : f(a,b), number of b : 1
def get_arg_number(code, var):
    if code.find(',')==-1:
        return "0"
    else:
        t=code.split('(')
        a=(t[1]).split(')')[0]
        t=a.split(',')
        for i in range(len(t)):
            j=t[i].replace(' ','')
            if j==var:
                return str(i)

#get the name of the argument of a function with it number of argument
def get_arg_name(f,num):
    res = j.runGremlinQuery("g.V().has('type','Function').has('name','"+f+"')")
    node="0"
    for r in res:
        s=str(r)
        node = get_id(s)
    res = j.runGremlinQuery("g.v("+node+").out().has('type','FunctionDef')")
    for r in res:
        s=str(r)
        code = (((get_opt(s))[1]).split(':'))[1]
        name = ((((code[0:-2]).split('('))[1]).split(','))[int(num)]
        if name[0]==' ':
            name=name[1:]
        name = (name.split(' '))[1]
        return name
    return "none"

#test if a variable is in the code with the format  : "code C"
def test_var_is_in_code(code,var):
    nb = code.count(var)
    if nb==0:
        return False
    else:
        for i in range(nb):
            place = code.find(var)
            a=code[place-1]
            if a==' ' or a=='"' or a=='=' or a=='>' or a=='<' or a=='+' or a=='-' or a=='/' or a=='*' or a=='[' or a=='(' or a=='{' or a==',' or a==';':
                a=code[place+len(var)]
                if a==' ' or a=='"' or a=='=' or a=='>' or a=='<' or a=='+' or a=='-' or a=='/' or a=='*' or a==']' or a==')' or a=='}' or a==',' or a==';':
                    return True
            code=code.replace(var,"_",1)
    return False

#get the "trace" of a variable in a function
#a "trace" has the following patern : [(function,name_arg_in_fun),...][line_var_use1,...]
def get_trace(fun,var,reste_fun):
    fun_id = get_fun_id(fun)
    reste_fun = get_reste_fun(reste_fun,fun_id)
    #print "\n\n"
    list_fun_trace=[]
    list_lines = []
    res = j.runGremlinQuery("g.V()")
    for r in res:
        s=str(r)
        if s.find("location")!=-1 and s.find("childNum")!=-1:
            code = (((get_opt(s))[1]).split(':'))[1]
            if test_var_is_in_code(code,var):
                l = s.find("location")
                line = ((s[l:]).split('"'))[1]
                line = line.split(':')
                l = s.find("functionId")
                line_fun_id = int(((((s[l:]).split(','))[0]).split(":"))[1])
                if line_fun_id==int(fun_id) and line[0]!=0:
                    list_lines.append(line[0])
                    #var niv+1 dans des fonctions : f(a) avec a UC
                    for f in reste_fun:
                        a = f[0]+" ("
                        if code.find(a)!=-1:
                            if code[1]==a[0]:
                                num_arg = get_arg_number(code,var)
                                name_arg = get_arg_name(f[0],num_arg)
                                list_fun_trace.append((f[0],name_arg))
                    #var niv+1 dans des calculs : a = b+1 avec b UC
                    if code.find("=")!=-1 and (code.find("==")==-1 or code.find("!=")==-1 or code.find(">=")==-1 or code.find("<=")==-1):
                        c = ((code.split("="))[0])[1:]
                        if c[-1]==" ":
                            c=c[0:-1]
                        if c.find(" ")!=-1:
                            c= (c.split(" "))[1]
                        if c.find(var)==-1:
                            list_fun_trace.append((fun,c))
    return (list_fun_trace,list_lines)

#PB : if a string contains the name of the variable, we will match that line too

#get the name of a function with it line
def get_fun_with_line(line):
    res = j.runGremlinQuery("g.V().has('type','Function')")
    for r in res:
        s=str(r)
        if s.find("location")!=-1:
            l = s.find("location")
            li = ((s[l:]).split('"'))[1]
            li = li.split(':')
            if int(li[0])==int(line):
                l = s.find("name")
                fun_name = (((((s[l:]).split(','))[0]).split(":"))[1])[1:-1]
                return fun_name

#get the line of a function with it name
def get_fun_with_line(line):
    res = j.runGremlinQuery("g.V().has('type','Function')")
    for r in res:
        s=str(r)
        if s.find("location")!=-1:
            l = s.find("location")
            li = ((s[l:]).split('"'))[1]
            li = li.split(':')
            if int(li[0])==int(line):
                l = s.find("name")
                fun_name = (((((s[l:]).split(','))[0]).split(":"))[1])[1:-1]
                return fun_name

#not finished, get the name of the file of the function
def get_fun_file(node):
    res = j.runGremlinQuery('g.v('+str(node)+').in()')
    file = "None"
    for r in file:
        s = str(r)
    return file

#get the UC variable and there function stored in a json format
def get_fun_vars():
    FUNs=[]
    funs=[]
    var=False
    fun="None"
    fun_i=0
    F = open("output.txt","r")
    i = -1
    for l in F.readlines():
        if len(l)>8 and l[0:8]=="fonction":
            line=(l.split(':'))[1]
            fun=get_fun_with_line(line)
            fun_line=int(line[1:-1])
            var = True
            FUNs.append([fun,[],0,fun_line])
        if len(l)>8 and l[0:8]=="variable":
            if var:
                FUNs[i][1].append(((l.split(':'))[1])[1:-1])
                var=False
            else:
                FUNs.append([fun,[],0,fun_line])
                FUNs[i][1].append(((l.split(':'))[1])[1:-1])
    return FUNs

#get the UC variable and there function stored in a json format
def get_fun_vars_json():
    FUNs=[]
    F = open("output.json","r").read()
    data = json.loads(F)
    vars = data["data"]
    for v in vars:
        fun_line=int(v["function line"])
        fun_name=get_fun_with_line(fun_line)
        prof=0
        name=unicodedata.normalize('NFKD', v["variable"]).encode('ascii','ignore')
        FUNs.append([fun_name,[name],0,fun_line])
    return FUNs

def main():
    Funs = get_fun_vars_json()
    tab_fun = []
    functions =  j.runGremlinQuery('getFunctionsByName("*")')
    for f in functions:
        s=str(f)
        s1 = (get_opt(s))[1][5:]
        s1=s1[1:-1]
        tab_fun.append((s1,get_fun_id(s1)))

    print "Step 1 done"

    stri=""
    prof=0
    fun_json=[]
    for f in Funs:
        prof=f[2]
        if prof>50:
            break
        stri=stri+"function:"+f[0]+"\n"
        stri=stri+"depth:"+str(prof)+"\n"
        vars=[]
        for v in f[1]:
            stri=stri+"variable:"+v+"\nlines:{"
            trace = get_trace(f[0],v,tab_fun)
            lines=[]
            if(len(trace[0])!=0):
                for a in trace[0]:
                    Funs.append([a[0],[a[1]],prof+1])
            for l in trace[1]:
                stri=stri+str(l)+","
                lines.append(l)
            stri=stri[0:-1]+"}\n"
            vars.append({"name":v,"lines":lines})
        fun_json.append({"function":f[0],"depth":prof,"variable":vars,"file":get_fun_file(get_fun_id(f[0]))})
        stri=stri+"\n"
    print "Step 2 done"
    print "\n\n"+stri+"\n\n"

    JSON={"data":fun_json}
    JSON_F = open("output_trace.json","w")
    JSON_F.write(json.dumps(JSON))
    JSON_F.close()

main()
