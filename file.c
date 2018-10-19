#include <stdlib.h>
#include <stdio.h>

struct TypeA {
  uint8_t type;
  uint8_t amt;
}

uint8_t get_type(File *fp){
  return fgetc(fp);
}

uint8_t get_typeA_amt(File *fp){
  return fgetc(fp);
}

int get_typeB_amt(File *fp){
  char amt[3];
  fread(amt, sizeof(char), 3, fp);
  int amt_no;
  i = 0;
  while (i < 3){
    if (amt[i] < 48 && amt[i] > 57){
      return -1;
    }
    i++;
  }
  sscanf(amt, "%d", &amt_no);
  return 1;
}

uint8_t read_typeA_num(File *fp, uint16_t num, uint8_t amt){
  TypeA structA;
  fread(num, sizeof(uint16_t), amt*2,fp);
}

int read_typeB_len(File *fp, uint8_t allcommas){
  uint8_t curr;
  int len;
  int pos = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  int size = ftell(fp);
  fseek(fp, pos, SEEK_SET);
  int commas = 0;
  long int prev = -1;
  while (1){
    curr = fgetc(fp);
    long int location= ftell(fp);
    if ( (commas == allcommas) && (curr == 1 || curr == 0) ){
      fseek(fp, -1, SEEK_CUR);
      break;
    } else if (curr < 48 && curr > 57){
        return -1;
    } else if (curr == ','){
      if (location-1 == prev){
        return -1;
      }
      prev = location;
      commas++;
    }
    len++;
    if (location == size){
      break;
    }

  }
  fseek(fp, pos, SEEK_SET);
  return len;

}



void read_typeB_num(FILE *fp, uint8_t *buff, uint8_t amt){
  fread(buff,sizeof(uint8_t),amt,fp);
}

void convert_typeA_to_typeB(uint8_t *AtoB, uint16_t *numA, uint8_t amt){
  int pos = 0;
  uint8_t type = 1;
  memset(AtoB, 0, 10000);
  memcpy(AtoB, &type, 1);
  pos++;
  char amt_B[3];
  int i = 2;
  while ( i >= 0 ){
    amt_B[i] = (amt % 10)+48;
    amount = amount/10;
  }
  char delimit = ',';
  memcpy(AtoB+pos, amt_B, 3);
  pos = pos + 3;
  i = 0;
  uint16_t write_num;
  while (i < amt) {
     write_num = (numA[i] << 8) | (numA[i] >> 8);
     char num_arr[6];
     int num_size = snprintf(num_arr, 6, "%d", write_num);
     memcpy(AtoB+pos, num_arr, num_size);
     pos = pos + num_size;
     if (i < amt - 1){
       memcpy(AtoB+pos, &delimit, 1);
       pos++;
     }
  }

}

void convert_typeA_to_typeA(uint8_t *typeA_typeA, uint16_t *numA, uint8_t amt){
  int size = 0;
  uint8_t type = 0;
  memcpy(typeA_typeA + size, &type, 1);
  size++;
  memcpy(typeA_typeA + size, &amt, 1);
  size++;
  memcpy(typeA_typeA + size, &numA, amt * 2);
}

void convert_typeB_to_typeA(uint8_t *typeB_typeA, uint8_t *numB, int num_len, uint8_t amt){

  char arr_B[amt][5];
  uint8_t type = 0;
  memset(arr_B, 0, sizeof(char) * amt * 5);
  int i = num_len - 1;
  int x = amt - 1;
  int y = 4;
  int prev_i;
  while (x >= 0){
    while (y >= 0){
      if (numB[i] != ','){
        arr_B[x][y] = numB[i];
        i--;
        if (i < 0){
          prev_i = y;
          break;
        }
      }else{
        i--;
        if (prev_i == 0){
          x++;
        }
        prev_i = y;
        break;
      }
    }
    prev_i = y;
  }

  uint16_t curr[amt];
  uint16_t this_item;
  int s = 0;
  while (s < amt){
    sscanf(arr_B[s], "%d", &this_item);
    curr[s] = (this_item << 8 | this_item >> 8);
  }
  memcpy(typeB_typeA, &type, 1);
  memcpy(typeB_typeA+1, $amt, 1);
  memcpy(typeB_typeA+2, curr, amt*2);

}

void convert_typeB_to_typeB(uint8_t *typeB_typeB, uint8_t *numB, char *amt, int num_len){
  int location = 0;
  uint8_t type = 1;
  memcpy(typeB_typeB+location, &type, 1);
  location++;
  memcpy(typeB_typeB+location, amt, 3);
  location = location + 3;
  memcpy(typeB_typeB+location, numB, num_len);
}

void print_nums_typeB(uint8_t *buffer, long int len){
  int i = 0;
  while (i < len){
    printf("%c", buffer[i]);
  }
  printf("\n");
}

void print_nums_typeA(uint8_t *buffer, uint8_t amt){
  int i = 0;
  while (i < amt){
    printf("%c", buffer[i]);
  }
  printf("\n");
}


int main() {
  FILE *fp;
  FILE *wptr;
  int to_format;
  fp = fopen("practice_project_test_file_1", "r+");
  long int location = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  long int size = ftell(fp);
  fseek(fp, location, SEEK_SET);

  uint8_t type;
  uint8_t amt;
  bool flag;
  while (size != ftell(fp)) {
    type = get_type(fp);
    if (type == 0){
      amt = get_typeA_amt(fp);
      uint16_t buff[amt];
      read_typeA_num(fp, buff, amt);
      print_nums_typeA(buff, amt);
      if (to_format == 0 || to_format == 2){
        size = 2*(amt + 1);
        uint8_t typeA_typeA[size];
        convert_typeA_to_typeA(typeA_typeA, buff, amt);
      }else if (to_format == 1 || to_format == 3){
        convert_typeA_to_typeB(buff, amt, wptr);
      }

    }else if (type == 1){
      uint16_t amt_of_B;
      char amt_typeB[3];
      int valid = get_typeB_amt(fp, amt_typeB);
      if (valid < 1){
        flag = true;
        break;
      }
      sscanf(amt_typeB, "%d", amt_of_B);
      long int len_B = read_typeB_len(fp, amt_of_B - 1, size);
      if (len_B == -1){
        flag = true;
        break;
      }
      uint8_t buff_B[len_B];
      read_typeB_num(fp, buff_B, len_B);
      print_nums_typeB(buff_B, len_B);
      if (to_format == 2 || to_format == 3){
        size = 2*(amt+1);
        uint8_t typeB_typeA[size];
        convert_typeB_to_typeA(typeB_typeA, buff_B, amt_of_B, len_B);
        fwrite(typeB_typeA, sizeof(uint8_t), size, wptr);

      }else if (to_format == 0 || to_format == 1){
        size = 4 + len_B;
        uint8_t typeB_typeB[size];
        convert_typeB_to_typeB(typeB_typeB, buff_B, amt_of_B, len_B);
        fwrite(typeB_typeB, sizeof(uint8_t), size, wptr);
      }

    }
    else{
      flag = true;

    }
  }
  return flag;


}
