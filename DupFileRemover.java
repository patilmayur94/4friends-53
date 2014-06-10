/*
 Duplicate file remover by 4friends
*/


import java.io.*;
import java.util.*;

class FileData 
{
	File file;
	String type;
	long size;
	public FileData(File f,String s,long l)
	{
		file=f;
		size=l;
		type=s;
	}
}

public class DupFileRemover 
{
	static int count=0,dupcount=0,cmp=0;
	static FileData totalfile[]=new FileData[9999999];
	static void process(File f)                                                                              //recursive method to get all files on given drive
	 {
		if(f.isFile())
		{
		  
			String s1=f.getName();
			if(!s1.startsWith("$"))                                                 //i.e not a system file
			{
				int n=s1.lastIndexOf(".");
				String ftype=s1.substring(n+1);
				totalfile[count]=new FileData(f,ftype,f.length());
				count++;
			}
		}
		  else if(f.isDirectory())                                                            //directory hence need to expand
		{
            if(!f.getName().startsWith("$"))                                                                  //i.e not a system directory
            {
			File fllist[]=f.listFiles(); // to get all files in directory
			if(fllist!=null && fllist.length>0)
			{
			for(int j=0;j<fllist.length;j++)
				process(fllist[j]);
			}
            }
		}
		else
		{
			System.out.println("not Access");
	    }
  }

 public static void main(String[] args)
 { 
	  File[] drive=File.listRoots();                                                                // to get all drive including dvd and pd
	  if(drive!=null && drive.length>0)
	     {
	        for(int i=0;i<drive.length;i++)
	         {
	    	      if(i!=0 && drive[i].getTotalSpace()!=0)                          // not considering c drive
	    	           {
	    	                String s=drive[i].getAbsolutePath();
	    	                File f=new File(s);
	    	                File dir[]=f.listFiles();
	    	                for(int j=0;j<dir.length;j++)
	    	                   {
	    	    	               String s1=dir[j].getName();
	    		                   if(dir[j].isFile())
	    		                       {
	    			                      if(!s1.startsWith("$"))
	    			                          {
	    			                            int n=s1.lastIndexOf(".");
	    			                            String ftype=s1.substring(n+1);
	    			            	             totalfile[count]=new FileData(dir[j],ftype,dir[j].length());
	    			  				             count++;
	    		                              }
	    		                       }
	    		                   else if(dir[j].isDirectory())
	    		                       {	
	    			                       process(dir[j]);
	    	                           }
	    		                   else
	    			                 continue;
	    	                   }
	    	           }
	         }
      }
	SortbySize sbs=new SortbySize();
	FileData totalfile2[]=new FileData[count];                                                                              //totalfile2 is the array of all file
	for(int i=0;i<count;i++)
		totalfile2[i]=totalfile[i];
	Arrays.sort(totalfile2,sbs);//files are sorted by size 
	for(int i=0;i<count;i++)
	{
		for(int j=i+1;j<count && totalfile2[i].size==totalfile2[j].size;j++)
		{
			if(totalfile2[i].type.equals(totalfile2[j].type))                                            // files are compare if having same type and same size
				{
					cmp++;
				    int result=compareFiles(totalfile2[i].file,totalfile2[j].file);
					if(result==1)
					{
						System.out.println(totalfile2[i].file +"is same as"+totalfile2[j].file);
						/*totalfile2[j].file.delete();*/                              //Remove comment to delete duplicate file
						dupcount++;
					}
						
				}
		}
	}
	System.out.println("total file(s):"+count);
	System.out.println("total comparision(s):"+cmp);
	System.out.println("total duplicate file(s)"+dupcount);
 }
 public static int compareFiles(File f1,File f2)                                                                             //function to compare two files content wise
 {
	 try
	 {
	 InputStream is1=new FileInputStream(f1);
	 InputStream is2=new FileInputStream(f2);
	 int c1=is1.read();
	 int c2=is2.read();
	 while(c1!=-1 && c2!=-1)
	 {
		if(c1!=c2)
		{
			is1.close();
			is2.close();
			return 0;
		}
		else
		{
			c1=is1.read();
			c2=is2.read();
		}
	 }
	 if(c1==c2)
	 {
		 is1.close();
		 is2.close();
		 return 1;
	 }
	 is1.close();
	 is2.close();
	 return 0;
	 }
	 catch(Exception e)
	 {
		 System.out.println(e.getMessage());
		 return 0;
	 }
 }
}
class SortbySize implements Comparator<FileData>
{
	public int compare(FileData arg0, FileData arg1) 
	{
		return (int) (arg0.size-arg1.size);
	}
}
