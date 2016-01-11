#include "structure.h"

vector<string> split(string str, string del){
    vector<string> arr;
    while(str!=del && str!="")
    {
    	/*if(cut>0)
    	{
    		if(arr.size()==cut)
    		{
    			arr.push_back(str);
    			return arr;
    		}
    	}*/
        if(str.find(del)>str.length())
        {
            arr.push_back(str);
            break;
        }
        if(str.substr(0, str.find(del)).length())
            arr.push_back(str.substr(0, str.find(del)));
        if((str.find(del)+del.length())<str.length())
            str=str.substr(str.find(del)+del.length());
        else
            str="";
    }
    return arr;
}


void structure::print(){
	
	//printing job info
	cout<<"engineer,"<<engineer<<endl;
    cout<<"date,"<<date<<endl;
    cout<<"width,"<<width<<endl;
    cout<<"unit,"<<unit<<endl;
    //cout<<"group:\t"<<group<<endl;
    
    //printing joint coordinates
    cout<<"JOINT COORDINATES:\n";
    cout<<"joint id , x cooordinates, y coordinates, z coordinates \n";
    for(int i=0;i<job_joints.size();i++)
    {
        cout<<job_joints[i].id<<","<<job_joints[i].x<<",";
        cout<<job_joints[i].y<<","<<job_joints[i].z<<endl;
    }
    
    //printing member incidences
    cout<<"MEMBER INCIDENCES:\n";
    cout<<"member id , joint id ,joint id , .... \n";
    for(int i=0;i<job_members.size();i++)
    {
        cout<<job_members[i].id<<",";
        for(int j=0;j<job_members[i].joint_id.size();j++)
        {
            cout<<job_members[i].joint_id[j]<<",";
        }
        cout<<endl;
    }
    
    //printing material defination
    cout<<"material definition:\n";
    cout<<"name,"<<job_material.name<<endl;
    cout<<"E,"<<job_material.E<<endl;
    cout<<"poison,"<<job_material.poison<<endl;
    cout<<"density,"<<job_material.density<<endl;
    cout<<"alpha,"<<job_material.alfa<<endl;
    cout<<"damp,"<<job_material.damp<<endl;
    cout<<"type,"<<job_material.type<<endl;
    cout<<"strength,"<<job_material.strength<<endl;
    cout<<"G,"<<job_material.G<<endl;
    
    //printing member property
    cout<<"MEMBER PROPERTY \n";
    cout<<"TYPE, YD, ZD, Member,  Member, Member, ...... \n";
    for(int i=0;i<member_pr.size();i++){
    	cout<<member_pr[i].type<<","<<member_pr[i].YD<<",";
    	cout<<member_pr[i].ZD;
    	for(int j=0;j<member_pr[i].joint_id.size();j++)
    		cout<<","<<member_pr[i].joint_id[j];
    	cout<<endl;
    }
    
    //printing concrete design info
    cout<<"CONCRETE DESIGN INFORMATION"<<endl;
    cout<<"CODE ,"<<con_des.code<<endl;
    for(int i=0;i<con_des.cty.size();i++){
    	cout<<con_des.cty[i].code<<","<<con_des.cty[i].section<<",";
    	cout<<endl;
    	for(int j=0;j<con_des.cty[i].member_id.size();j++)
    		cout<<con_des.cty[i].member_id[j]<<",";
    	cout<<endl;
    }
    
    //printing Design Beam
    cout<<"Design Beam"<<endl;
    for(int i=0;i<beam.size();i++){
    	cout<<beam[i]<<",";
    }
    
    //printing design column
    cout<<endl<<"DESIGN COLUMN"<<endl;
    for(int i=0;i<column.size();i++){
    	cout<<column[i]<<",";
    }
}

    
structure::structure(fstream &file)   
{
    string str="", temp, temp1;
    vector<string> temp2;
    vector<string> temp3;
    char ch;
    
    //replacing newline wiht space
    while(file.get(ch))
    {
        if(ch=='\n' || ch=='\r')
        	ch=' ';
        str+=ch;
    }
    
	//get enginner name 
    temp=split(str, "START JOB INFORMATION")[1];
    engineer=split(temp, "DATE")[0];
    
    //get date
    temp=split(temp, "DATE ")[1];
    date=split(temp, "END JOB INFORMATION")[0];
    
    //get width
    temp=split(temp, "END JOB INFORMATION")[1];
    width=split(split(temp, "UNIT")[0], "WIDTH")[1];
    
    //get units
    temp=split(temp, "UNIT")[1];
    unit=split(temp, "JOINT COORDINATES")[0];
    
    //get joint coordinates
    temp=split(temp, "JOINT COORDINATES")[1];
    temp1=split(temp, "MEMBER INCIDENCES")[0];
    temp2=split(temp1, "; ");
    for(int i=0;i<temp2.size()-1;i++)
    {
        temp3=split(temp2[i], " ");
        joint j;
        istringstream(temp3[0])>>j.id;
        istringstream(temp3[1])>>j.x;
        istringstream(temp3[2])>>j.y;
        istringstream(temp3[3])>>j.z;
        job_joints.push_back(j);
        temp3.clear();
    }
    
    
    temp2.clear();
    
    //get MEMBER INCIDENCES
    temp=split(temp, "MEMBER INCIDENCES")[1];
    temp1=split(temp, "START GROUP DEFINITION")[0];
    temp2=split(temp1, ";");
    for(int i=0;i<temp2.size()-1;i++)
    {	
        temp3=split(temp2[i], " ");
        member m;
        istringstream(temp3[0])>>m.id;
        for(int j=1;j<temp3.size();j++)
        {
            int l;
            istringstream(temp3[j])>>l;
            m.joint_id.push_back(l);
        }
        job_members.push_back(m);
        temp3.clear();
    }
    temp2.clear();
    
    //getting group information 
    temp=split(temp, "START GROUP DEFINITION")[1];
    group=split(temp, "END GROUP DEFINITION")[0];
    
    //getting material info
    temp=split(temp, "END GROUP DEFINITION")[1];
    temp=split(temp, "DEFINE MATERIAL START")[1];
    temp1=split(temp, "END DEFINE MATERIAL")[0];
    job_material.name=split(temp1, " E ")[0];
    temp1=split(temp1, " E ")[1];
   	job_material.E=split(temp1, "POISSON")[0];
   	temp1=split(temp1, "POISSON")[1];
    istringstream(split(temp1, "DENSITY")[0])>>job_material.poison;
    temp1=split(temp1, "DENSITY")[1];
    istringstream(split(temp1, "ALPHA")[0])>>job_material.density;
    temp1=split(temp1, "ALPHA")[1];
    job_material.alfa=split(temp1, "DAMP")[0];
    temp1=split(temp1, "DAMP")[1];
    istringstream(split(temp1, "TYPE")[0])>>job_material.damp;
    temp1=split(temp1, "TYPE")[1];
    job_material.type=split(temp1, "STRENGTH")[0];
    temp1=split(temp1, "STRENGTH")[1];
    job_material.strength=split(temp1, "G")[0];
    temp1=split(temp1, "G")[1];
    job_material.G=temp1;
    
    
    //gettin member properties
    temp=split(temp, "END DEFINE MATERIAL")[1];
    temp1=split(temp, "MEMBER PROPERTY INDIAN  ")[1];
    temp2=split(temp1," ");
    mem_pro *me;
    me=new mem_pro;
    for(int i=0; i<temp2.size();i++){
    	float l=0,l1=0;
    	if(isdigit(temp2[i][0])){
    		istringstream(temp2[i])>>l;
    		(*me).joint_id.push_back(l);
    		continue;
    	}
    	if(temp2[i]=="TO"){
    		istringstream(temp2[i-1])>>l;
    		istringstream(temp2[i+1])>>l1;
    		for( int j=l+1;j<l1;j++){
    			(*me).joint_id.push_back(j);
    		}
    	}
		else{
			if(isalpha(temp2[i][0])){
				istringstream(temp2[i+2])>>l;
				istringstream(temp2[i+4])>>l1;
				(*me).type=temp2[i];
				(*me).YD=l;
				(*me).ZD=l1;
				member_pr.push_back(*me);
				me=new mem_pro;
				i=i+4;
				if(!isdigit(temp2[i+1][0]))
					break;
			}
		}
	}
    
    
    //getting concrete info 
    temp1=split(temp,"START CONCRETE DESIGN")[1];
    temp1=split(temp1,"DESIGN BEAM")[0];
    temp2=split(temp1," ");
    con_des.code=temp2[1];
    code_type *cd;
    cd=new code_type;
	for(int i=2; i<temp2.size();i++){
		float l=0,l1=0;
		if(isdigit(temp2[i][0])){
			istringstream(temp2[i])>>l;
			cd->member_id.push_back(l);
			continue;
		}
		if(temp2[i]=="TO"){
			istringstream(temp2[i-1])>>l;
			istringstream(temp2[i+1])>>l1;
			for( int j=l+1;j<l1;j++){
					cd->member_id.push_back(j);
				}
		}
		else{
			if(isalpha(temp2[i][0])){
				if(i>5){
				  	con_des.cty.push_back(*cd);
				}
				cd=new code_type;
				cd->code=temp2[i];
				cd->section=temp2[i+1];
				i=i+2;
			}
		}
	}
   con_des.cty.push_back(*cd);
    
   
   	//getting design beam 
	temp1=split(temp,"DESIGN BEAM")[1];
	temp1=split(temp1,"DESIGN COLUMN")[0];
	temp2=split(temp1," ");
	for(int i=0; i<temp2.size();i++){
		float l=0,l1=0;
		if(isdigit(temp2[i][0])){
			istringstream(temp2[i])>>l;
			beam.push_back(l);
			continue;
		}
		if(temp2[i]=="TO"){
			istringstream(temp2[i-1])>>l;
			istringstream(temp2[i+1])>>l1;
			for( int j=l+1;j<l1;j++){
				beam.push_back(j);
			}
		}
	}
	
	//getting design column
    temp1=split(temp,"DESIGN COLUMN")[1];
    temp2=split(temp1," ");
    for(int i=0; i<temp2.size();i++){
		float l=0,l1=0;
		if(isdigit(temp2[i][0])){
			istringstream(temp2[i])>>l;
			column.push_back(l);
			continue;
		}
		if(temp2[i]=="TO"){
			istringstream(temp2[i-1])>>l;
			istringstream(temp2[i+1])>>l1;
			for( int j=l+1;j<l1;j++){
				column.push_back(j);
			}
		}
	}
}
