#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX 50
#define MAX_ROWS 60
typedef struct cart 
{
  int IID;
  char NAME[MAX];
  int PRICE;
  int QTY;
  struct cart *link;
}CART;
typedef struct prod_dets 
{
    int PID;
    int IID;
    char NAME[MAX];
    int PRICE;
    char EXPDATE[MAX];
    int QTY;
    struct prod_dets *right;
}PRODUCT;
typedef struct category_dets 
{
    int slno;
    int qty;
    struct prod_dets *head;
    struct category_dets *down;
}CATEGORY;
void insert(CATEGORY *a, PRODUCT *prods, int start, int end) 
{
    a->head = NULL;
    PRODUCT *current = NULL;
    a->qty=0;
    for (int i = start; i <= end; i++) 
	{
        PRODUCT *temp = malloc(sizeof(PRODUCT));
        temp->PID = prods[i].PID;
        temp->IID = prods[i].IID;
        strcpy(temp->NAME, prods[i].NAME);
        temp->PRICE = prods[i].PRICE;
        strcpy(temp->EXPDATE, prods[i].EXPDATE);
        temp->QTY = prods[i].QTY;
        temp->right = NULL;
        if (current == NULL) 
		{
            a->head = temp;
            current = temp;
        } 
        else 
        {
            current->right = temp;
            current = temp;
        }
        a->qty++;
    }
}
void swap(PRODUCT *a, PRODUCT *b) 
{
    int tempPID = a->PID;
    int tempIID = a->IID;
    int tempPRICE = a->PRICE;
    int tempQTY = a->QTY;
    char tempNAME[MAX];
    char tempEXPDATE[MAX];
    strcpy(tempNAME, a->NAME);
    strcpy(tempEXPDATE, a->EXPDATE);
    a->PID = b->PID;
    a->IID = b->IID;
    a->PRICE = b->PRICE;
    a->QTY = b->QTY;
    strcpy(a->NAME, b->NAME);
    strcpy(a->EXPDATE, b->EXPDATE); 
    b->PID = tempPID;
    b->IID = tempIID;
    b->PRICE = tempPRICE;
    b->QTY = tempQTY;
    strcpy(b->NAME, tempNAME);
    strcpy(b->EXPDATE, tempEXPDATE);
}
int daysBetweenDates(const char* date1, const char* date2) 
{
    struct tm tm1, tm2;
    memset(&tm1, 0, sizeof(struct tm));
    memset(&tm2, 0, sizeof(struct tm));
    // Parse date1
    if (sscanf(date1, "%d-%d-%d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year) != 3) 
	{
        fprintf(stderr, "Failed to parse date1\n");
        return -1;
    } 
    // Parse date2
    if (sscanf(date2, "%d-%d-%d", &tm2.tm_mday, &tm2.tm_mon, &tm2.tm_year) != 3) 
	{
        fprintf(stderr, "Failed to parse date2\n");
        return -1;
    }

    // Adjust year and month values
    tm1.tm_year -= 1900; // Adjust year to years since 1900
    tm1.tm_mon--;       // Adjust month to 0-based

    tm2.tm_year -= 1900;
    tm2.tm_mon--;

    // Convert struct tm to time_t
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    if (t1 == -1 || t2 == -1) 
	{
        perror("mktime");
        return -1;
    }

    // Calculate the difference in seconds
    double difference = difftime(t2, t1);

    // Convert seconds to days
    int days = (int)(difference / (60 * 60 * 24));

    return days;
}
void sorting(CATEGORY *arrpArray[], int arrptr_size)
{
    struct prod_dets *a=arrpArray[arrptr_size]->head;
    struct prod_dets *b;
    while(a->right!=NULL)
    {
        b=a->right;
        while(b!=NULL)
        {
            int first=daysBetweenDates("09-11-2023",a->EXPDATE);
            int second=daysBetweenDates("09-11-2023",b->EXPDATE);
            if(first>second)
            {
                swap(b,a);
            }
            b=b->right;
        }
        a=a->right;
    }
}
int printArray(CATEGORY *arrpArray[], int arrptr_size, struct cart **head) 
{
	int iid, quantity;
	printf("Details for slno %d:\n", arrpArray[arrptr_size]->slno);
	struct prod_dets *current = arrpArray[arrptr_size]->head;
	while (current != NULL) 
	{
		printf("PID: %d, IID: %d, NAME: %s, PRICE: %d, EXPDATE: %s, QTY: %d\n",current->PID, current->IID, current->NAME, current->PRICE, current->EXPDATE, current->QTY);
		current = current->right;
	}
	printf("----------------------------------------------------\n");
	printf("Enter the IID(item id) you would like to add to your cart and the corresponding quantity:\n");
	scanf("%d %d", &iid, &quantity);
	struct prod_dets *current_cart = arrpArray[arrptr_size]->head;
	while (current_cart != NULL) 
	{
		if (iid == current_cart->IID) 
		{
			if (quantity <= current_cart->QTY) 
			{
				struct cart *cart_items = malloc(sizeof(struct cart));
				cart_items->IID = current_cart->IID;
				strcpy(cart_items->NAME, current_cart->NAME);
				cart_items->PRICE = current_cart->PRICE;
				cart_items->QTY = quantity;
				current_cart->QTY -= quantity;
				if (*head == NULL) 
					*head = cart_items;
				else 
				{
					cart_items->link = *head;
					*head = cart_items;
				}
                printf("Item successfully added to cart\n");
                return 1;
			} 
			else 
			{
				printf("Insufficient quantity available for IID %d\n", iid);
			}
		}
		current_cart = current_cart->right;
	}
    
}
void search_item(CATEGORY *arrpArray[],char *choice,int n)
{
	int flag=0;
	for(int i=0;i<n;i++)
	{
		PRODUCT *cur=arrpArray[i]->head;
		while(cur!=NULL)
		{
			if((strcasecmp(cur->NAME,choice)==0))
			{
				flag=1;
				printf("PID: %d, IID: %d, NAME: %s, PRICE: %d, EXPDATE: %s, QTY: %d\n",cur->PID, cur->IID, cur->NAME, cur->PRICE, cur->EXPDATE, cur->QTY);
			}
			cur=cur->right;
		}	
	}
	if(flag==0)
			printf("Sorry!Item not available\n");
}
void update(CATEGORY *arrpArray[],int x,char *name,int n)
{
    int y;
    for(int i=0;i<n;i++)
    {
        struct prod_dets *cur=arrpArray[i]->head;
        while(cur!=NULL)
        {
            if(strcasecmp(cur->NAME,name)==0)
            {
                if(x==1)
                {
                    printf("Enter updated price");
                    scanf("%d",&y);
                    cur->PRICE=y;
                    printf("Price updated!!\n");
                }
                else if (x==2)
                {
                    printf("Enter updated qty:");
                    scanf("%d",&y);
                    cur->QTY=y;
                    printf("Quantity updated!!\n");
                }
            }
            cur=cur->right;
        }
    }
}
void price(CART *head_cart) 
{
    int cost = 0;  
    CART *ptr = head_cart;
    if (ptr == NULL) 
    {
        printf("Cart is empty\n");
        printf("Leaving price function\n");
        return;
    }
    while (ptr!= NULL) 
    {
        cost = cost + (ptr->PRICE * ptr->QTY);
        ptr = ptr->link;
    }
    printf("Final cost is %d\n", cost);
}
void delete(struct cart **head_cart, int iid) 
{
    int qty;
    CART *ptr = *head_cart;
    CART *prev = NULL;
    if (ptr == NULL) 
    {
        printf("Cart is empty\n");
        return;
    }
    if (ptr->IID == iid) 
    {
        printf("Enter the quantity you would like to delete\n");
        scanf("%d", &qty);
        ptr->QTY = ptr->QTY - qty;
        if (ptr->QTY < 0) 
        {
            printf("Please enter the correct value\n");
            ptr->QTY = ptr->QTY + qty;
        } 
        else 
        {
            printf("item updated\n");
        }
        return;
    }
    while (ptr != NULL && ptr->IID != iid) 
    {
        prev = ptr;
        ptr = ptr->link;
    }
    if (ptr != NULL) 
    {
        printf("Enter the quantity you would like to delete\n");
        scanf("%d", &qty);
        ptr->QTY = ptr->QTY - qty;
        if (ptr->QTY < 0) 
        {
            printf("Please enter the correct value\n");
            ptr->QTY = ptr->QTY + qty;
        } 
        else 
        {     
            printf("Item updated\n");
        }
    } 
    else 
    {
        printf("Item with IID %d is not found in the cart.\n", iid);
    }
}
void display(CART *head_cart) 
{
    CART *ptr = head_cart;
    if (ptr == NULL) 
    {
        printf("Cart is empty, please add items to view\n");
        return;
    } 
    else 
    {
        printf("IID\tNAME\t\tPRICE\tQTY\n");
        while (ptr != NULL) 
        {
            printf("%d\t%s\t\t%d\t%d\n", ptr->IID, ptr->NAME, ptr->PRICE, ptr->QTY);
            ptr = ptr->link;     
        }  
    }
    return;
}
void printArray_admin(CATEGORY *arrpArray[], int arrptr_size) 
{
	printf("Details for slno %d:\n", arrpArray[arrptr_size]->slno);
	struct prod_dets *current = arrpArray[arrptr_size]->head;
	while (current != NULL) 
	{
		printf("PID: %d, IID: %d, NAME: %s, PRICE: %d, EXPDATE: %s, QTY: %d\n",current->PID, current->IID, current->NAME, current->PRICE, current->EXPDATE, current->QTY);
		current = current->right;
	}
	printf("----------------------------------------------------\n");
}
void deleteadmin(CATEGORY *arrpArray[], int arrptr_size) 
{   
    int id,iid;
    printf("Details for slno %d:\n", arrpArray[arrptr_size]->slno);
    PRODUCT *current = arrpArray[arrptr_size]->head;
    while (current != NULL) 
	{
        printf("PID: %d, IID: %d, NAME: %s, PRICE: %d, EXPDATE: %s, QTY: %d\n",
               current->PID, current->IID, current->NAME, current->PRICE, current->EXPDATE, current->QTY);
        current = current->right;
    }
    printf("----------------------------------------------------\n");
    printf("Enter the PID and the IID of the element you wish to delete:");
    scanf("%d %d",&id,&iid);
    PRODUCT *current1 = arrpArray[id-1]->head;
    PRODUCT *prev=NULL;
    while(current1!=NULL && current1->IID!=iid)
    {
        prev=current1;
        current1=current1->right;
    }
    if (current1 != NULL) 
    {
        if (prev == NULL) 
        {
          
            arrpArray[id - 1]->head = current1->right;
        } 
        else 
        {
            prev->right = current1->right;
        }
        free(current1);
        printf("Item with IID %d deleted from  list %d.\n", iid, id);
    } 
    else 
    {
        printf("Item with IID %d not found in slno %d.\n", iid, id);
    }
}
int count=3;
int main()
{
	struct cart *head_cart = NULL;
	int arrptr_size = 5;
    CATEGORY *arrpArray[arrptr_size];
    for (int i = 0; i < arrptr_size; i++) 
	{
        arrpArray[i] = (CATEGORY *)malloc(sizeof(CATEGORY));
        arrpArray[i]->slno = i + 1;
        arrpArray[i]->qty = 0;
        arrpArray[i]->head = NULL;
        arrpArray[i]->down = (i == arrptr_size - 1) ? NULL : arrpArray[i + 1];
    }
    FILE *file = fopen("final_csv.csv", "r");
    if (!file)
	{
        printf("Error in opening the file\n");
        return 1;
    }
    struct prod_dets prods[MAX_ROWS];
    int rowCount = 0;
    char line[MAX];
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file) != NULL && rowCount < MAX_ROWS) 
	{
        int result = sscanf(line, "%d,%d,%49[^,],%d,%49[^,],%d",
                            &prods[rowCount].PID,
                            &prods[rowCount].IID,
                            prods[rowCount].NAME,
                            &prods[rowCount].PRICE,
                            prods[rowCount].EXPDATE,
                            &prods[rowCount].QTY);
        if (result == 6) 
		{
            rowCount++;
        } 
		else 
		{
            printf("Error reading line %d\n", rowCount + 1);
            printf("Read: %s", line);
        }
    }
    fclose(file);
	for (int i = 0; i < arrptr_size; i++)
	{
        int start = i * (rowCount / arrptr_size);
        int end = (i == arrptr_size - 1) ? rowCount - 1 : (i + 1) * (rowCount / arrptr_size) - 1;
        insert(arrpArray[i], prods, start, end);
    }
	int ua,u_choice,a_choice,select,res=0;
	char pwd[10],item[100];
	while(1)
	{
		printf("1.User login\n2.Admin login\n0.To exit\n");
		printf("Who are you?");
		scanf("%d",&ua);
		switch(ua)
		{
			case 1:	while(1)
					{
						printf("1.Display product details\n2.Search for a product\n3.View your cart\n4.Delete items from cart\n5.Checkout\n0.To exit user login\n");
						printf("Enter your choice:");
						scanf("%d",&u_choice);
						if(u_choice==1)
						{	
							while (1) 
							{
								printf("1.Body products\n2.Food products\n3.Cosmetics\n4.Stationary products\n5.Kitchen Products\n0.To go back to main menu\n");
								printf("What are you looking for?");
								scanf("%d", &select);
								if (select >= 1 && select <= arrptr_size)
								{
                                    sorting(arrpArray, select - 1);
									res=printArray(arrpArray, select - 1, &head_cart);
                                    if(res==0)
                                    {
                                        printf("Sorry item not present\n");
                                    }
								} 
								else 
								{
									printf("Invalid input. Exiting... Bye\n");
									break;
								}
							}
						}			
						else if(u_choice==2)
						{	
							printf("Enter item  to be searched:");
							scanf("%s",item);
							search_item(arrpArray,item,arrptr_size);
						}
						else if(u_choice==3)
						{
                            display(head_cart);
                        }
                        else if(u_choice==4)
						{
                            int iid;
                            printf("Please enter the IID of the element you wish to remove");
                            scanf("%d",&iid);
                            delete(&head_cart,iid);
                        }
                        else if(u_choice==5)
						{
                            price(head_cart);
                        }
						else
						{
							printf("Sorry, try again!!\n");
							break;
						}						
					}
					break;
			case 2:	printf("Enter password:");
					scanf("%s",pwd);
					if(strcmp("admin@123",pwd)==0)
					{
						while(1)
						{
							printf("1.Update items\n2.Display all products\n3.Delete items\n0.To logout\n");
							printf("Enter your choice for admin:");
							scanf("%d",&a_choice);
							if(a_choice==1)
							{
								int x;
								char name[50];
								printf("Enter item to be updated:");
								scanf("%s",name);
                                printf("1.To update price\n2.To update quantity\n");
								printf("What do you want to update");
								scanf("%d",&x);
								update(arrpArray,x,name,arrptr_size);
							}
							else if(a_choice==2)
							{
								while (1) 
								{
									printf("1.Body products\n2.Food products\n3.Cosmetics\n4.Stationary products\n5.Kitchen Products\n0.To go back to main menu\n");
									printf("What are you looking for?");
									scanf("%d", &select);
									if (select >= 1 && select <= arrptr_size)
									{
                                        sorting(arrpArray, select - 1);
										printArray_admin(arrpArray, select - 1);
									} 
									else 
									{
										printf("Invalid input. Exiting... Bye\n");
										break;
									}
								}
							}
                            else if(a_choice==3)
                            {
                                printf("1.Body products\n2.Food products\n3.Cosmetics\n4.Stationary products\n5.Kitchen Products\n6.Exit\n");
                                printf("What are you looking for?:");
                                scanf("%d", &select);
                                if (select >= 1 && select <= arrptr_size)
                                {
                                    deleteadmin(arrpArray, select - 1);
                                }
                                else
                                {
                                    printf("Invalid input. Exiting... Bye\n");
                                    break;
                                }

                            }
                            else
                                break;
                        }
					}
					else
                    {
						printf("Password incorrect!\nTry again!\n");
                        count--;
                        printf("%d\n",count);
                        if(count==0)
                        {
                            printf("You have exceeded the try limit. Please come back again later");
                            return 0;
                        }
                    }
					break;
			default:exit(0);
		}
	}
}