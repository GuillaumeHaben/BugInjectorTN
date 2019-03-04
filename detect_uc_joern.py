from joern.all import JoernSteps

j = JoernSteps()

j.setGraphDbURL('http://localhost:7474/db/data/')

# j.addStepsDir('Use this to inject utility traversals')

j.connectToDatabase()

#res =  j.runGremlinQuery('getFunctionsByName("main")')
#res = j.runGremlinQuery('getFunctionsByName("m*")')
#res = j.runGremlinQuery('getFunctionsByParameter("*r*")')
#on peut aussi faire .id par exemple a la fin pour avoir juste un truc
#res = j.runGremlinQuery('getCallsTo("memcpy").functions()')
#res = j.runGremlinQuery('g.v(0).out()')
#res = j.runGremlinQuery('getCallsTo("error")')


def get_opt(node):
    opt = (node.split('{'))[1]
    opt = (opt.split('}'))[0]
    opts = []
    code=""
    c=False
    for i in opt.split(','):
        if i.find("code")!=-1:
            code=i
            c=True
        elif c:
            if i.find("functionId")!=-1:
                c=False
                opts.append(code)
                code=""
                opts.append(i)
            else:
                code=code+","+i

        else:
            opts.append(i)
    return opts

def get_id(s):
    s1 = (s.split('{'))[0][2:-1]
    return s1

def get_fun_id(fun):
    res = j.runGremlinQuery("g.V().has('type','Function').has('name','"+fun+"')")
    for r in res:
        #s=str(r)
        #s1 = (s.split('{'))[0][2:-1]
        return get_id(str(r))
    return "0"

def get_reste_fun(tab,funid):
    t = []
    for i in tab:
        if int(i[1])!=int(funid):
            t.append(i)
    return t

def get_sons(id):
    res = j.runGremlinQuery("g.v("+str(id)+").out()")
    ret = []
    for r in res:
        ret.append(str(r))
    return ret

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

def get_trace(fun,var,reste_fun):
    fun_id = get_fun_id(fun)
    reste_fun = get_reste_fun(reste_fun,fun_id)
    """res = j.runGremlinQuery("g.V().has('code','"+var+"')")
    for r in res:
        s=str(r)
        i = s.find("functionId")
        if i!=-1:
            id = s[(i+11):]
            id = (id.split(','))[0]
            if int(id)==int(fun_id):
                #print s
                s=s"""
    #print "\n\n"
    list_fun_trace=[]
    list_lines = []
    res = j.runGremlinQuery("g.V()")
    for r in res:
        s=str(r)
        if s.find("location")!=-1:
            if s.find(var)!=-1:
                code = (((get_opt(s))[1]).split(':'))[1]
                l = s.find("location")
                line = ((s[l:]).split('"'))[1]
                line = line.split(':')
                l = s.find("functionId")
                line_fun_id = int(((((s[l:]).split(','))[0]).split(":"))[1])
                if line_fun_id==int(fun_id):
                    list_lines.append(line[0])
                for f in reste_fun:
                    a = f[0]+" ("
                    if code.find(a)!=-1:
                        if code[1]==a[0]:
                            num_arg = get_arg_number(code,var)
                            name_arg = get_arg_name(f[0],num_arg)
                            list_fun_trace.append((f[0],name_arg))
    """print list_fun_trace
    print list_lines"""
    return (list_fun_trace,list_lines)

#PB : si y a un string qui contient le nom de la variable, ba on va matcher aussi cette ligne

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

def get_fun_vars():
    FUNs=[]
    funs=[]
    F = open("output.txt","r")
    i = -1
    for l in F.readlines():
        if len(l)>8 and l[0:8]=="fonction":
            f=get_fun_with_line((l.split(':'))[1])
            if(f not in funs):
                FUNs.append([f,[]])
                funs.append(f)
                i+=1
        if len(l)>8 and l[0:8]=="variable":
            FUNs[i][1].append(((l.split(':'))[1])[1:-1])
    return FUNs

def main():
    Funs = get_fun_vars()
    tab_fun = []
    functions =  j.runGremlinQuery('getFunctionsByName("*")')
    for f in functions:
        s=str(f)
        s1 = (get_opt(s))[1][5:]
        s1=s1[1:-1]
        tab_fun.append((s1,get_fun_id(s1)))

    stri=""
    for f in Funs:
        stri=stri+"function:"+f[0]+"\n"
        for v in f[1]:
            stri=stri+"variable:"+v+"\nlignes:{"
            trace = get_trace(f[0],v,tab_fun)
            if(len(trace[0])!=0):
                for a in trace[0]:
                    Funs.append([a[0],[a[1]]])
            for l in trace[1]:
                stri=stri+str(l)+","
            stri=stri[0:-1]+"}\n"
        stri=stri+"\n"
    print "\n\n\n\n"+stri

    F = open("output_trace.txt","w")
    F.write(stri)

main()
