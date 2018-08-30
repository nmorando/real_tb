#include <stdio.h>
#include <stdlib.h>

void  func_FSM (int kernel_size, int image_size ,int stride,int size_buffer, int depth_kernel,int kernel_pack, int number_pe)
;
int memory[300047];
int flag[300047];
int imm[3][4][4];
FILE *f;
int n_ram_access=0;
int clk_ram=0;
int clk_tot=0;
float energy_tot=0;
float pe_energy=0.00000135;
float ram_energy=0.228;
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
 
    func_FSM(11, 277,4,64000,3,96,121);
    clk_tot+=clk_ram;
    printf("n_tot_clk:%d\n",clk_tot*96);
    printf("n_ram_clk:%d\n",clk_ram*96);
    printf("n_ram_access:%d\n",n_ram_access*96);
    printf("energy_tot:%f\n",energy_tot*96);
    printf("power_consuption_max:%f\n",pe_energy*121);




    
    
    return 0;
    
}




// TODO WORKS FOR CHAIN PE
void func_FSM (int kernel_size, int image_size ,int stride,int size_buffer, int depth_kernel,int kernel_pack,int number_pe)
{
    f= fopen("testbench_pe.txt", "w");
    int parti_da_questo_counter_di_memoria=0;
    int quanti_kernel_hai_gia_elaborato=0;
    int i,j,k,z,for_pe_counter;
    int quanto_ci_metti_per_una_riga=0;// compute how many access in ram you need for completing the
                                        // convolution row
    
    int tmp_memoria=0;
    int quante_cicli_completi_per_convoluzione=0;// compute how many time you have to do the convolution row
                                                 // in order to complete the matrix convolution
    int quanti_righe_complete_ci_stanno_in_memoria=0;
    int quanti_serie_pe_ci_stanno_per_kernel=number_pe/(kernel_size*kernel_size);
    
    quanto_ci_metti_per_una_riga=kernel_size*image_size;
    
    quante_cicli_completi_per_convoluzione=(image_size-kernel_size)/stride+1;
    
    quanti_righe_complete_ci_stanno_in_memoria=size_buffer/quanto_ci_metti_per_una_riga;
    
    int confronto_fine_conv;
    if(quante_cicli_completi_per_convoluzione%quanti_righe_complete_ci_stanno_in_memoria!=0)
     confronto_fine_conv=quante_cicli_completi_per_convoluzione/quanti_righe_complete_ci_stanno_in_memoria+1;// dice quanto dovró accedere in memoria per completare convoluzione
    else
        confronto_fine_conv=quante_cicli_completi_per_convoluzione/quanti_righe_complete_ci_stanno_in_memoria;
    if(quanti_righe_complete_ci_stanno_in_memoria>quante_cicli_completi_per_convoluzione)
    {
        confronto_fine_conv=1;
        quanti_righe_complete_ci_stanno_in_memoria=quante_cicli_completi_per_convoluzione;
    }
    
    int save_counter;
    for(k=0;k<depth_kernel;k++)
        for(i=0;i<image_size;i++)
            for(j=0;j<image_size;j++)
            imm[k][i][j]=rand()%256;
    
    
    int starting=0,counter=0;
    for(for_pe_counter=0;for_pe_counter<depth_kernel;for_pe_counter++)
    {
        for(j=0;j<size_buffer;j++)
        {
            flag[j]=0;
        }
        starting=0;
        for(j=0;j<quanti_serie_pe_ci_stanno_per_kernel;j++)
        {
        for(i=0;i<kernel_size*kernel_size;i++)
        {
            fprintf(f,"PELOAD PE[%d],%d\n ",i,(rand()%3)-1);
            clk_tot++;
            energy_tot+=(float)number_pe*pe_energy;
            
        }
            quanti_kernel_hai_gia_elaborato++;
        }
        while(starting+kernel_size<image_size)
        {
    for(z=0;z<confronto_fine_conv;z++)
    {
        for(j=0;j<size_buffer;j++)
        {
            flag[j]=0;
        }
    for(k=0;k<quanti_righe_complete_ci_stanno_in_memoria;k++)
    {
        
        for(i=0;i<image_size;i++)
    {
        for(j=starting;j<kernel_size+starting;j++)
        {
            if((j-starting)>stride-1)
            {
                if(((j-starting)/stride)==1)
                {
                    if(flag[counter]==0)
                    {
                    memory[counter]=imm[for_pe_counter][j][i];
                    fprintf(f,"LOAD memory[%d],imm[%d][%d][%d]\n ",counter,for_pe_counter,j,i);
                        flag[counter]=1;
                        energy_tot+=ram_energy;
                        save_counter=counter;
                        counter= counter+(quanto_ci_metti_per_una_riga)-1;
                        if((counter<=size_buffer))
                        {
                            
                            memory[counter]=imm[for_pe_counter][j][i];
                            fprintf(f,"RELOAD memory[%d]\n ",counter);
                            flag[counter]=1;
                        }
                        counter=save_counter;
                        n_ram_access++;
                        clk_ram+=2;
                    }
                    
                    

                    
                    
                    
                }
                
                if(((j-starting)/stride)==2)
                {
                    if(flag[counter]==0)
                    {
                        memory[counter]=imm[for_pe_counter][j][i];
                        fprintf(f,"LOAD memory[%d],imm[%d][%d][%d]\n ",counter,for_pe_counter,j,i);
                        flag[counter]=1;
                        energy_tot+=ram_energy;
                        save_counter=counter;
                        counter= counter+(quanto_ci_metti_per_una_riga)-1;
                        if(counter<=size_buffer)
                        {
                            memory[counter]=imm[for_pe_counter][j][i];
                            flag[counter]=1;
                            fprintf(f,"RELOAD memory[%d]\n ",counter);
                        }
                        counter= counter+(quanto_ci_metti_per_una_riga)-1;
                        if((counter<=size_buffer))
                        {
                            memory[counter]=imm[for_pe_counter][j][i];
                            flag[counter]=1;
                            fprintf(f,"RELOAD memory[%d]\n ",counter);
                        }
                        counter=save_counter;
                        n_ram_access++;
                        clk_ram+=2;
                    }
                    
                        
                   
                    
                    
                   
                    

                    
                }
                
                if(((j-starting)/stride)==3)
                {
                    if(flag[counter]==0)
                    {
                        energy_tot+=ram_energy;
                        memory[counter]=imm[for_pe_counter][j][i];
                        fprintf(f,"LOAD memory[%d],imm[%d][%d][%d]\n ",counter,for_pe_counter,j,i);
                        flag[counter]=1;
                        save_counter=counter;
                        counter= counter+(quanto_ci_metti_per_una_riga)-1;
                        if((flag[counter]==0)&&(counter<=size_buffer))
                        {
                            memory[counter]=imm[for_pe_counter][j][i];
                            flag[counter]=1;
                            fprintf(f,"RELOAD memory[%d]\n ",counter);
                        }
                        counter= counter+(quanto_ci_metti_per_una_riga)-1;

                        if((flag[counter]==0)&&(counter<=size_buffer))
                        {
                            memory[counter]=imm[k][j][i];
                            flag[counter]=1;
                            fprintf(f,"RELOAD memory[%d]\n ",counter);
                        }
                        
                        counter= counter+(quanto_ci_metti_per_una_riga)-1;
                        if((flag[counter]==0)&&(counter<=size_buffer))
                        {
                            memory[counter]=imm[k][j][i];
                            fprintf(f,"RELOAD memory[%d]\n ",counter);
                        }
                        
                        counter=save_counter;
                        n_ram_access++;
                        clk_ram+=2;
                    }
                    
                    
                    
                    
                    
                    
                    
                    
                }
                
                counter++;
            }
            else
            {
                if(flag[counter]!=1)
                {
                    energy_tot+=ram_energy;
                    flag[counter]=1;
                    memory[counter]=imm[for_pe_counter][j][i];
                    fprintf(f,"LOAD memory[%d],imm[%d][%d][%d]\n ",counter,for_pe_counter,j,i);
                    n_ram_access++;
                    clk_ram+=2;
                }
                
            counter++;
            }
            

            
        }
        
        
        
        
    }
        starting=starting+stride;
    }
    
        // credo vada qua
        
        counter=0;
        for(j=0;j<quanti_righe_complete_ci_stanno_in_memoria;j++)
        {
        fprintf(f, "ACT %d\n",quanto_ci_metti_per_una_riga);
            energy_tot+=(float)number_pe*pe_energy*quanto_ci_metti_per_una_riga;
            clk_tot+=quanto_ci_metti_per_una_riga;

            for(k=0;k<quanto_ci_metti_per_una_riga;k++)
            {
        for(i=0;i<2*kernel_size*kernel_size;i++)
        {
            fprintf(f,"NOP\n");
        }
        if(quanti_serie_pe_ci_stanno_per_kernel>1)
        {
             fprintf(f, "STORE&SUM memory1\n");
        }
        else
        {
            fprintf(f, "STORE memory1\n");

            
        }
       
        for(i=0;i<kernel_size;i++)
        {
             fprintf(f,"LOAD memory[%d]\n ",parti_da_questo_counter_di_memoria);
            
        }
                if(quanti_serie_pe_ci_stanno_per_kernel>1)
                {
                    fprintf(f, "STORE&SUM memory1\n");
                }
                else
                {
                    fprintf(f, "STORE memory1\n");
                    
                    
                }
        }
        }
        
        
        
    }
        }
    ///ccc
    }
}
