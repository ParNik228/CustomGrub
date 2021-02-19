#include <QFile>
#include <QTextStream>
#include <string>
#include <iostream>
#include <QDir>

QString Other;
int check =0;
bool savecheck =false;


void LoadGrubBackup(){

    QFile file("../CustomGrub/GrubBackup/grubdefault");
    file.copy("../CustomGrub/cpgrub");
    file.close();

}

void MakeGrubBackup(){

    QFile file("../CustomGrub/GrubBackup/grubdefault");
    file.remove();
    file.close();

    QFile fl("/boot/grub/grub.cfg");
    QDir dir("../CustomGrub");
    dir.mkdir("GrubBackup");
    fl.copy("../CustomGrub/GrubBackup/grubdefault");
    fl.close();


}

void FileRemove(){

    QFile fl("../CustomGrub/cpgrub");
    fl.remove();
    fl.close();

}

void SaveGrubParam(QString otherparam,QString Module){

    QString locdir= "../CustomGrub";
    QDir dir(locdir);
    dir.mkdir("GrubBackup");
    QFile fl("../CustomGrub/GrubBackup/grubsaved");

    if (fl.open(QIODevice::WriteOnly | QIODevice::Append)) {

        QTextStream out(&fl);
        if(savecheck==false){
            out<<Other;
            out<<Module;
            savecheck=true;

        }else{
            out<<Module;
        }

        fl.close();
    }

}

QList<QString> Optionfill(QString NameMenu){

    QFile fl("../CustomGrub/ReadyParamList/ReadyParameterList");
    if (!fl.open(QIODevice::ReadOnly)){

        qWarning("Cannot open file for reading");

    }
    QList<QString> list;
    QTextStream in(&fl);
    QString line;
    QString str;
    while(!in.atEnd()){

        line=in.readLine();
        if(line[0]=="\t"){
            if(str==NameMenu){
                line.remove("\n");
                list +=line.remove("\t");
            }


        }else{
            str=line.remove("\n");

        }


    }
    fl.remove();
    fl.close();
    return list;

}

void SaveToFileOptionList(QString NameMenu,QString NameParam,QString NameCore,QString NInsmod,QString NSetOptions,QString NLinuxDir,QString NInitrdDir){

    QFile fl("../CustomGrub/ReadyParamList/ReadyParameterList");

    if (fl.open(QIODevice::WriteOnly | QIODevice::Append)) {

        QTextStream out(&fl);
        out<<NameMenu+"\n";
        out<<"\t"+NameParam+"\n";
        out<<"\t"+NameCore+"\n";
        out<<"\t"+NInsmod+"\n";
        out<<"\t"+NSetOptions+"\n";
        out<<"\t"+NLinuxDir+"\n";
        out<<"\t"+NInitrdDir+"\n";

        fl.close();
    }


}

int ParamCounter(QString CoreName){
    QString str;
    int strcount=0;
    QFile fl("../CustomGrub/ReadyParamList/ReadyParameterList");
    if (!fl.open(QIODevice::ReadOnly)){

        qWarning("Cannot open file for reading");

    }
    QTextStream in(&fl);

    QString line;
    QString param;
    while(!in.atEnd()){

        line=in.readLine();
        if(line.remove("\n")==CoreName){

            strcount++;

        }
      /*  for(int i=0;i<line.length();i++){

            if(i==line.length()-1||line[i]=="\n"){
                str +=line[i];
                param=str.remove("\n");
                str="";
                if(param==CoreName){
                    strcount++;

                }
            }
            str +=line[i];
        }*/



    }
    fl.close();
    return strcount;

}

void CheckDirAndFile(){

    QString locdir= "../CustomGrub";
    QString filedir="../CustomGrub/ReadyParamList/ReadyParameterList";
    QDir dir(locdir);
    dir.mkdir("ReadyParamList");

    QFile fl(filedir);


    if (fl.open(QIODevice::WriteOnly | QIODevice::Append)) {

        fl.close();
    }
}

QString UpdateTextModule(QString TextModule,QString newValue,QString oldValue,int way,QString param){

    if(way==1){
        QString nonSpaceStr;
        QList<QString> newparam;
        QList<QString> oldparam;
        if(newValue==""){

            if(oldValue==""){
                return TextModule;
            }else {
                int len=oldValue.length();

                for(int i=0;i<oldValue.length();i++){
                    if(oldValue[i]==" "){
                        oldparam+=nonSpaceStr;
                        nonSpaceStr="";
                    }
                    if(i==len-1){
                        nonSpaceStr +=oldValue[i];
                        oldparam+=nonSpaceStr;
                        nonSpaceStr="";
                    }
                    nonSpaceStr+=oldValue[i];

                }
                for(int i =0;i<oldparam.size();i++){
                    newparam+="";

                        TextModule=TextModule.replace("\t"+param+" "+oldparam[i]+"\n",newparam[i]);

                }
                return TextModule;

            }

        }

        /*if(oldValue[oldValue.length()-1]==" "){
            for(int i=0;i<oldValue.length()-1;i++){
                nonSpaceStr +=oldValue[i];
            }
            oldValue=nonSpaceStr;
            nonSpaceStr="";
        }
        if(newValue[newValue.length()-1]==" "){
            for(int i=0;i<newValue.length()-1;i++){
                nonSpaceStr +=newValue[i];
            }
            newValue=nonSpaceStr;
        }

        QList<QString> newparam = newValue.split(" ");
        QList<QString> oldparam = oldValue.split(" ");
        if(oldparam.size()>newparam.size()){

            for (int i=0;i<oldparam.size()-newparam.size();i++){
                newparam +="";
            }
            for(int i =0;i<oldparam.size();i++){
                if(newparam[i] ==""){
                    TextModule=TextModule.replace("\t"+param+" "+oldparam[i]+"\n",newparam[i]);
                }else{
                    TextModule=TextModule.replace(param+" "+oldparam[i],param+" "+newparam[i]);
                }

            }
            return TextModule;

        }
        if(oldparam.size()==newparam.size()){
            for(int i =0;i<oldparam.size();i++){
                TextModule=TextModule.replace(param+" "+oldparam[i],param+" "+newparam[i]);
            }
            return TextModule;
        }
        if(oldparam.size()<newparam.size()){
            QString str;
            for(int i =0;i<oldparam.size();i++){

                if(i==oldparam.size()-1){
                    for(int j =i;j<newparam.size();j++){

                        str +="\t"+param+" "+newparam[j]+"\n";

                    }
                    TextModule=TextModule.replace("\t"+param+" "+oldparam[i]+"\n",str);

                }else{
                    TextModule=TextModule.replace(param+" "+oldparam[i],param+" "+newparam[i]);
                }

            }
            return TextModule;


        }*/
    }
    if(way == 2){

        oldValue.remove("\t");
        TextModule = TextModule.replace(param+"\t"+oldValue,param+"\t"+newValue);
        return TextModule;
    }
    if(way == 3){

        TextModule = TextModule.replace("'"+oldValue+"'","'"+newValue+"'");
        return TextModule;
    }

}

QString DirSearch(QString TextModule,QString opt1,QString opt2,QString opt3,QString opt4,QString opt5){

    QString var;
    QString str;
    QString sstr;
    bool ins =false;

    for(int i=0;i<TextModule.length();i++){

        str+=TextModule[i];
        if(str.length()>2){
            sstr = str[str.length()-1];

            if(sstr =="\n"){
                str="";

            }
            sstr="";
        }
        if(ins){
           // if(TextModule[i]==" "){

           // }else{
                var+=TextModule[i];
                if(str==""){
                    var.remove("\n");
                    //var +=" ";
                    ins=false;
                    return var;
            //}

            }

        }

        if (str == opt1||str==opt2||str==opt3||str==opt4||str==opt5){

            ins=true;

        }



    }
    return var;

}

QString OptionFill(QString TextModule,QString opt1,QString opt2,QString opt3,QString opt4,QString opt5){

    QString var;
    QString str;
    QString sstr;
    bool ins =false;

    for(int i=0;i<TextModule.length();i++){

        str+=TextModule[i];
        if(str.length()>2){
            sstr = str[str.length()-1];

            if(sstr =="\n"){
                str="";

            }
            sstr="";
        }
        if(ins){
            if(TextModule[i]==" "){

            }else{
                var+=TextModule[i];
                if(str==""){
                    var.remove("\n");
                    var +=" ";
                    ins=false;
            }

            }

        }

        if (str == opt1||str==opt2||str==opt3||str==opt4||str==opt5){

            ins=true;

        }



    }
    return var;

}

QString NameMenuEntryFill(QString TextModule){

    QString Name;
    QChar ch;
    int start=0;
    int end=0;

    for (int i=0;i<TextModule.length();i++){
        ch=TextModule[i];
        if(ch==" "&&TextModule[i+1]=="'"){
            start=i+2;
        }
        if(ch==" "&&TextModule[i+1]=="-"){
            end=i-1;
            break;
        }
    }
    for(int i=start;i<end;i++){

        Name +=TextModule[i];

    }
    return Name;
}

QString OtherFill(QString TextModule){
    if(check>0){
        Other.remove(TextModule);
        return Other;
    }
    else{

        QFile fl("../CustomGrub/cpgrub");
        if (!fl.open(QIODevice::ReadOnly)){

            qWarning("Cannot open file for reading");

        }
        QTextStream in(&fl);

        QString line;
     //   QString str;

        while (!in.atEnd()){

            line = in.readLine();

            Other +=line+"\n";

        }
        check++;
        Other.remove(TextModule);
        //fl.remove();
        fl.close();
        return Other;
    }

}

QString TextModuleFill(int menuid){

    QFile fl("../CustomGrub/cpgrub");
    if (!fl.open(QIODevice::ReadOnly)){

        qWarning("Cannot open file for reading");

    }
    int strid = 1;
    QString str;
    QString TextModule;

        QTextStream in(&fl);
        while (!in.atEnd()) {

            QString line = in.readLine();

            if(str=="menuentry "|str=="\tmenuentry "){

            }
            else{
                str="";
            }
            for (int i=0;i<line.length();i++){

                if(str=="menuentry "||str=="\tmenuentry "){

                    break;
                }
                else{
                    str +=line[i];
                }

            }
            if(str=="menuentry "||str=="\tmenuentry "){
            if(strid<menuid){
                strid++;
                str="";
            }else{
                if(strid==menuid){
                    if(line =="}"||line=="\t}"){


                        TextModule +=line+"\n";
                        str="";
                        return TextModule;



                    }
                    else{

                            TextModule +=line+"\n";

                    }
                }
            }
            }

        }
        fl.close();
        return TextModule;
}

int StructCounter (QFile &fl){
    int numberofStruct =0;

    QTextStream in(&fl);
    while (!in.atEnd()) {

        QString line = in.readLine();
        QString str;

        for (int i=0;i<line.length();i++){

            if(line[i]==" "){
                if(str == "menuentry"||str=="\tmenuentry"){
                    numberofStruct++;
                    break;
                }
            }
            else{
                str +=line[i];
            }

        }

    }
    fl.close();
    return numberofStruct;

}
