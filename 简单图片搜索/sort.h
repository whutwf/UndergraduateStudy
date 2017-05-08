
#ifndef SORT_H
#define SORT_H
  /*交换函数，作用是交换数组中的两个元素的位置*/
    void swap(double array[806][2],int i,int j)
    {
        double tmp;
	tmp = array[i][0];
        array[i][0]=array[j][0];
        array[j][0]=tmp;

	tmp = array[i][1];
        array[i][1]=array[j][1];
        array[j][1]=tmp;
    }

/*冒泡排序*/
void BubbleSort(double array[806][2],int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=n-1;j>i;j--)
        {
            if(array[j][1]<array[j-1][1])
                swap(array,j,j-1);
        }
    }
}
#endif // SORT_H
