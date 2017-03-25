#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>




/// coded by GENIUS  people  - PRAMOD , YASH , SOHAM --------------------------------------------

void displaymat(double **mat,int r,int c){
	int i,j;
	for(i=0;i<r;i++){
		for(j=0;j<c;j++){
			printf("%f ",mat[i][j]);
		}
		printf("\n");
	}
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
}


double RandomNumber(float Min, float Max)
{
    return (((float)rand() / (float)RAND_MAX) * (Max - Min)) + Min;
}



void mat_mult_factor(double **A, double **B,double **C, int r1, int c1, int c2,double factor){
	for (int i=0;i<r1;i++){
		for (int k=0;k<c2;k++){
			double sum = 0.0;
			for (int j=0;j<c1;j++){
				sum += A[i][j]*B[j][k];
			}
			C[i][k] = sum*factor;
		}
	}
}

double sigmoid(double x){
	double y,exp_value;
	exp_value = exp(-x);
	y = 1/(1 + exp_value);
	return y;
}

void check_classification(double **output,double **target_feature_data, int rows,int classes,char c){
	double temp,accuracy=0.0;
	int index,original,correct=0;
	for (int i=0;i<rows;i++){
		temp = -1.0;
		
		for (int j=0;j<classes;j++){
			if (temp < output[i][j] ){	
				temp = output[i][j];
				index = j+1;
			}

			if (target_feature_data[i][j] != 0.00){
				
				original = j+1;
			}

		}
		if (original == index){
			accuracy += 1.0;
			correct ++;
		}
		
		printf("Data point (%d): Original_class : (%d)----classified_as : (%d)\n",i,original,index);
	}
	accuracy = (accuracy * 100) / ((double) rows);
	
								

	printf("Correctly_classified = %d\nWrongly_classified = %d\n",correct,rows-correct);
	if (c=='r')
		printf("Training_Accuracy(percent) = %.4f\n",accuracy);
	else if (c=='e')
	{											
		printf("Testing_Accuracy(percent) = %.4f\n",accuracy);
	}
}

double error_func(double **output, double **target_feature_data, int rows,int epochs){
	double temp=0.0,value = 0.0;

	for(int i=0;i<rows;i++){
		temp = 0.0;
		for(int j=0;j<3;j++){
			temp = temp + pow(output[i][j] - target_feature_data[i][j],2);
		}
		value = value + (0.5)*temp;
	}
	printf("Epoch : %d ---- Error : %f\n",epochs,value);
	return value;
}

void mat_sum(double **A, double **delta_A, int r1,int c1){
	for (int i=0;i<r1;i++){
		for(int j=0;j<c1;j++){
			A[i][j] += delta_A[i][j];
		}
	}
}

void mat_transpose(double **mat, double **mat_transpose, int r1,int c1){
	for (int i=0; i<r1; i++){
		for (int j=0;j<c1;j++){
			mat_transpose[j][i] = mat[i][j];
		}
	}
}


void test_on_testdata(double **w_ih, double **w_ho,double **test_feature_dupdata,double **test_target_data,int n){
int i=0,j=0,rows,columns;
	size_t count;
	
	char *line = malloc(100);
	char *pt;
	FILE *file;
	double error_value,max_x_value=-10000,min_x_value=10000;
	
	if (file = fopen("test.csv","r")){
	while(getline(&line,&count, file)!=-1){
		pt = strtok(line,",");
		j= 0;
		while(pt != NULL){
			float a = atof(pt);
			
			test_feature_dupdata[i][j] = a;
			if (a > max_x_value)
				max_x_value = a;
			if (a < min_x_value)
				min_x_value = a;

			pt = strtok(NULL, ",");
			if (pt == NULL)
				test_target_data[i][(int)a - 1] = 1.00; 
															// 				
			j++;
		}
		i++;
	}
	rows = i;
	columns = j;

	
	  for (int k = 0; k < rows; k++) {
			test_feature_dupdata[k][columns-1]=1.000000;
		for (int k1=0;k1<columns-1;k1++){
				test_feature_dupdata[k][k1] = ((test_feature_dupdata[k][k1]) - min_x_value) / (max_x_value - min_x_value); 
			}
	  }
	 

	double **test_hidden;
	test_hidden = (double **) malloc(rows * sizeof(double*));
	for (int row=0;row<rows;row++){
				test_hidden[row] = (double *) malloc((n+1)* sizeof(double));
	}

	double **test_output;
	test_output = (double **) malloc(rows * sizeof(double*));
	for (int row=0;row<rows;row++){
				test_output[row] = (double *) malloc(3* sizeof(double));
	}

	mat_mult_factor(test_feature_dupdata,w_ih,test_hidden,rows,columns,n+1,1.0);
	for (int i=0;i<rows;i++){
		test_hidden[i][n]=1.00; // set the bias input to 1.00 in the hidden layer...
		for (int j=0;j<n;j++){
			test_hidden[i][j] = sigmoid(test_hidden[i][j]); 
		}
	}

	mat_mult_factor(test_hidden,w_ho,test_output,rows,n+1,3,1.0);

	for (int i=0;i<rows;i++){
		for (int j=0;j<3;j++){
		test_output[i][j] = sigmoid(test_output[i][j]); 
		}
	}

	check_classification(test_output,test_target_data,rows,3,'e');
	 
	}
	else{
		printf("The file has not been found\n");
	}


}


int main(int argc, char *argv[]){
	printf("Welcome back to C , master KC....\n");

	int i=0,j=0,rows,columns;
	size_t count;
	

int ARR_SIZE = 200;

double **test_target_data; 
	double **test_feature_dupdata;
	//ARR_SIZE = 200;
	test_target_data = (double **) malloc(ARR_SIZE * sizeof(double*));
	test_feature_dupdata = (double **) malloc(ARR_SIZE * sizeof(double*));

	for (int row=0;row<ARR_SIZE;row++){
		test_target_data[row] = (double *) malloc(3 * sizeof(double)); 
	}

	for (int row=0;row<ARR_SIZE;row++){
	test_feature_dupdata[row] = (double *) malloc(ARR_SIZE * sizeof(double));
	}


	double **target_feature_data , **train_feature_dupdata;
	//int ARR_SIZE = 200;
	target_feature_data = (double **) malloc(ARR_SIZE * sizeof(double*));
	train_feature_dupdata = (double **) malloc(ARR_SIZE * sizeof(double*));
	for (int row=0;row<ARR_SIZE;row++){
		target_feature_data[row] = (double *) malloc(3 * sizeof(double)); // allocating 3 columns for each ARR_SIZE(200) rows....
	}

	for (int row=0;row<ARR_SIZE;row++){
		train_feature_dupdata[row] = (double *) malloc(ARR_SIZE * sizeof(double));
	}

	char *line = malloc(100);
	char *pt;
	FILE *file;
	double max_x_value=-10000,min_x_value=10000;
	// reading data from a file....
	if (file = fopen("train.csv","r")){
	while(getline(&line,&count, file)!=-1){
		pt = strtok(line,",");
		j= 0;
		while(pt != NULL){
			float a = atof(pt);
			//train_feature_data[i][j] = a;
			train_feature_dupdata[i][j] = a;
			if (a > max_x_value)
				max_x_value = a;
			if (a < min_x_value)
				min_x_value = a;

			pt = strtok(NULL, ",");
			if (pt == NULL)
				target_feature_data[i][(int)a - 1] = 1.00; // set target vector for a class : 
															// ex: for 2, target vector = [0,1,0]...				
			j++;
		}
		i++;
	}


	// printing the 2d array....
	rows = i;
	columns = j;


	  for (int k = 0; k < rows; k++) {
	  	
			train_feature_dupdata[k][columns-1]=1.000000;
			for (int k1=0;k1<columns-1;k1++){
				train_feature_dupdata[k][k1] = ((train_feature_dupdata[k][k1]) - min_x_value) / (max_x_value - min_x_value); 
			}
	  }

	displaymat(target_feature_data,rows,3);
	

	fclose(file);
	}
	else{
		printf("The file has not been found\n");
	}

	int n;
	printf("max_input_value = %f\nmin_input_value = %f\n",max_x_value,min_x_value);
	printf("Enter the number of hidden neurons(10-20) : ");
	scanf("%d",&n);
	
	double **hidden;
	hidden = (double **) malloc(rows * sizeof(double*));
	for (int row=0;row<rows;row++){
				hidden[row] = (double *) malloc((n+1)* sizeof(double));
	}

	
 	double **w_ih;
        int ARR_SIZE1 = 14;	

        w_ih = (double **) malloc(ARR_SIZE1 * sizeof(double*));
        for (int row=0;row<ARR_SIZE;row++){
                w_ih[row] = (double *) malloc((n+1)* sizeof(double));
        }


	

	for(int row=0;row<ARR_SIZE1;row++){
		for(int column=0;column<n+1;column++){
			double k=  RandomNumber(-2.0,2.0);
			w_ih[row][column]=k;

		}
	}


for(int u=0;u<ARR_SIZE1;u++){
	w_ih[u][n]=0.000000;
}
printf("\nprinting w_ih (14 x n+1)...\n");
displaymat(w_ih,ARR_SIZE1,n+1);




double **w_ho;
	//int ARR_SIZE2 = n+1;
	//int ko=3;
        w_ho = (double **) malloc((n+1) * sizeof(double*));
        
        for (int row=0;row<n+1;row++){
                w_ho[row] = (double *) malloc(3* sizeof(double));
        }


        for(int row=0;row<n+1;row++){
                for(int column=0;column<3;column++){
                        double k= RandomNumber(-2.0,2.0);// M + rand() / (RAND_MAX / (N - M + 1) + 1);
                        w_ho[row][column]=k;

                }


		}
printf("\nprinting w_ho (n+1 x 3)...\n");
displaymat(w_ho,n+1,3);


mat_mult_factor(train_feature_dupdata,w_ih,hidden,rows,ARR_SIZE1,n+1,1.0);

for (int i=0;i<rows;i++){
	hidden[i][n]=1.00;
	for (int j=0;j<n;j++){
		hidden[i][j] = sigmoid(hidden[i][j]); 
	}
}
printf("\nprinting hidden through activation (118 x n+1)...\n");
displaymat(hidden,rows,n+1);
////////////////////////////////////////////////////////////////////////////////////////

double **output;

	output = (double **) malloc((rows) * sizeof(double*));
	for (int row=0;row<rows;row++){
		output[row] = (double *) malloc(3* sizeof(double));
	}

mat_mult_factor(hidden,w_ho,output,rows,n+1,3,1.0);

for (int i=0;i<rows;i++){
	for (int j=0;j<3;j++){
		output[i][j] = sigmoid(output[i][j]); 
	}
}

printf("printing the output after first forward pass....\n\n");
displaymat(output,rows,3);
check_classification(output,target_feature_data,rows,3,'r');	



double error_value,error_threshold = 6.0,eta = 0.005;
int epochs = 1;
error_value = error_func(output,target_feature_data,rows,epochs);


double **hidden_transpose;
	hidden_transpose = (double **) malloc((n+1) * sizeof(double*));
	for (int row=0;row<n+1;row++){
				hidden_transpose[row] = (double *) malloc((rows)* sizeof(double));
	}

double **delta_k;
	delta_k = (double **) malloc( rows * sizeof(double*));
	for(int row=0;row<rows;row++){
		delta_k[row]= (double *) malloc(3 * sizeof(double));	
	}

double **delta_k_transpose;
	delta_k_transpose = (double **) malloc( 3 * sizeof(double*));
	for(int row=0;row<3;row++){
		delta_k_transpose[row]= (double *) malloc(rows * sizeof(double));	
	}

double **temp_w_del;
	temp_w_del = (double **) malloc( (n+1) * sizeof(double*));
	for(int row=0;row<n+1;row++){
		temp_w_del[row]= (double *) malloc(rows * sizeof(double));	
	}

double **delta_w_ih;
	delta_w_ih = (double **) malloc(ARR_SIZE1 * sizeof(double*));
	for(int row=0;row<ARR_SIZE1;row++){
		delta_w_ih[row]= (double *) malloc((n+1) * sizeof(double));	
	}

double **delta_w_ih_transpose;
	delta_w_ih_transpose = (double **) malloc((n+1) * sizeof(double*));
	for(int row=0;row<n+1;row++){
		delta_w_ih_transpose[row]= (double *) malloc(ARR_SIZE1 * sizeof(double));	
	}

double **delta_w_ho;
	delta_w_ho = (double **) malloc( (n+1) * sizeof(double*));
	for(int row=0;row<n+1;row++){
		delta_w_ho[row]= (double *) malloc(3 * sizeof(double));	
	}


while (error_value > error_threshold){

	mat_transpose(hidden,hidden_transpose,rows,n+1);
	for (int i=0;i<rows;i++){
		for (int j=0;j<3;j++){
			delta_k[i][j] = (target_feature_data[i][j] - output[i][j])*output[i][j]*(1-output[i][j]);
		}
	}

	mat_mult_factor(hidden_transpose,delta_k,delta_w_ho,n+1,rows,3,eta);
	
	mat_transpose(delta_k,delta_k_transpose,rows,3);
	mat_mult_factor(w_ho,delta_k_transpose,temp_w_del,n+1,3,rows,1.0);
	for(int i=0;i<n+1;i++){
		for(int j=0;j<rows;j++){
			temp_w_del[i][j] = temp_w_del[i][j]*hidden_transpose[i][j]*(1-hidden_transpose[i][j]);
		}
	}

	mat_mult_factor(temp_w_del,train_feature_dupdata,delta_w_ih_transpose,n+1,rows,ARR_SIZE1,eta);
	mat_transpose(delta_w_ih_transpose,delta_w_ih,n+1,ARR_SIZE1);

	mat_sum(w_ho,delta_w_ho,n+1,3);
	mat_sum(w_ih,delta_w_ih,ARR_SIZE1,n+1);
	


mat_mult_factor(train_feature_dupdata,w_ih,hidden,rows,ARR_SIZE1,n+1,1.0);

for (int i=0;i<rows;i++){
	hidden[i][n]=1.00; // set the bias input to 1.00 in the hidden layer...
	for (int j=0;j<n;j++){
		hidden[i][j] = sigmoid(hidden[i][j]); 
	}
}

mat_mult_factor(hidden,w_ho,output,rows,n+1,3,1.0);

for (int i=0;i<rows;i++){
	for (int j=0;j<3;j++){
		output[i][j] = sigmoid(output[i][j]); // pass the inputs of the hidden layer to the sigmoid function...
	}
}

error_value = error_func(output,target_feature_data,rows,epochs);

epochs ++;
}
check_classification(output,target_feature_data,rows,3,'r');

test_on_testdata(w_ih,w_ho,test_feature_dupdata,test_target_data,n);	

return 0;
}
