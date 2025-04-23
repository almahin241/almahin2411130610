#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float getDepreciationRate(int age) {
    if (age <= 1) return 0.10;
    else if (age == 2) return 0.20;
    else if (age == 3) return 0.30;
    else if (age == 4) return 0.40;
    else return 0.50;
}

float getBaseDutyRate(int cc) {
    if (cc <= 1500) return 1.31;
    else if (cc <= 2000) return 2.0;
    else if (cc <= 2500) return 2.75;
    else if (cc <= 3000) return 4.0;
    else return 8.26;
}

int main() {
    char name[50], filename[60];
    FILE *fp;
    int choice, isUsed, cc, age, strokeType;
    float baseValue, yellowBookValue, depreciationRate = 0;
    float depreciatedValue, importDuty, vat, advanceTax, regulatoryDuty, advTradeVat, totalTax;
    float baseDutyRate, supplementaryDuty, additionalTax;

    printf("Enter your name: ");
    scanf("%s", name);
    sprintf(filename, "%s_duty_report.txt", name);
    fp = fopen(filename, "a");

    fprintf(fp, "User: %s\n\n", name);

    printf("Select vehicle type to calculate duty:\n");
    printf("1. Car\n2. Motorcycle\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Is the car used? (1 for Yes, 0 for No): ");
        scanf("%d", &isUsed);
        printf("Enter Engine Capacity (cc): ");
        scanf("%d", &cc);

        if (isUsed) {
            printf("Enter Yellow Book Price (C&F value in Tk): ");
            scanf("%f", &yellowBookValue);
            printf("Enter Age of Car (in years): ");
            scanf("%d", &age);

            if (age > 4) {
                printf("Used cars older than 4 years are not allowed for import in Bangladesh.\n");
                fprintf(fp, "Car import not allowed (age > 4 years).\n");
                fclose(fp);
                return 0;
            }

            depreciationRate = getDepreciationRate(age);
            depreciatedValue = yellowBookValue * (1 - depreciationRate);
            baseDutyRate = getBaseDutyRate(cc);
            importDuty = depreciatedValue * baseDutyRate;
            advanceTax = yellowBookValue * 0.05;
            regulatoryDuty = depreciatedValue * 0.05;
            advTradeVat = depreciatedValue * 0.04;
            vat = (importDuty + regulatoryDuty + advanceTax + advTradeVat + depreciatedValue) * 0.15;
            totalTax = importDuty + regulatoryDuty + advanceTax + advTradeVat + vat;

            fprintf(fp, "--- Used Car Duty Breakdown ---\n");
            fprintf(fp, "Depreciated Value (after %.0f%% depreciation): Tk %.2f\n", depreciationRate * 100, depreciatedValue);
            fprintf(fp, "Import Duty (%.2f%%): Tk %.2f\n", baseDutyRate * 100, importDuty);
            fprintf(fp, "Regulatory Duty (5%%): Tk %.2f\n", regulatoryDuty);
            fprintf(fp, "Advance Tax (5%%): Tk %.2f\n", advanceTax);
            fprintf(fp, "Advance Trade VAT (4%%): Tk %.2f\n", advTradeVat);
            fprintf(fp, "VAT (15%%): Tk %.2f\n", vat);
            fprintf(fp, "Total Estimated Tax: Tk %.2f\n", totalTax);
        } else {
            printf("Enter Base Value of New Car (in Tk): ");
            scanf("%f", &baseValue);
            importDuty = baseValue * 0.25;
            regulatoryDuty = baseValue * 0.03;

            if (cc > 3000 && cc <= 4000) supplementaryDuty = baseValue * 5.0;
            else if (cc > 2000 && cc <= 3000) supplementaryDuty = baseValue * 2.5;
            else supplementaryDuty = baseValue * 1.0;

            additionalTax = (cc > 3500) ? 350000.0 : 0.0;
            vat = (baseValue + importDuty + regulatoryDuty + supplementaryDuty + additionalTax) * 0.15;
            totalTax = importDuty + regulatoryDuty + supplementaryDuty + vat + additionalTax;

            fprintf(fp, "--- New Car Duty Breakdown ---\n");
            fprintf(fp, "Import Duty (25%%): Tk %.2f\n", importDuty);
            fprintf(fp, "Regulatory Duty (3%%): Tk %.2f\n", regulatoryDuty);
            fprintf(fp, "Supplementary Duty: Tk %.2f\n", supplementaryDuty);
            fprintf(fp, "Additional Tax: Tk %.2f\n", additionalTax);
            fprintf(fp, "VAT (15%%): Tk %.2f\n", vat);
            fprintf(fp, "Total Estimated Tax: Tk %.2f\n", totalTax);
        }
    } else if (choice == 2) {
        printf("Enter Engine Capacity (cc): ");
        scanf("%d", &cc);
        printf("Enter Stroke Type (1 for 4-stroke, 2 for 2-stroke): ");
        scanf("%d", &strokeType);
        printf("Enter C&F value of bike (in Tk): ");
        scanf("%f", &baseValue);

        importDuty = baseValue * 0.45;

        if (strokeType == 1) {
            if (cc <= 250) supplementaryDuty = baseValue * 0.60;
            else supplementaryDuty = baseValue * 1.00;
        } else {
            supplementaryDuty = baseValue * 2.50;
        }

        vat = (baseValue + importDuty + supplementaryDuty) * 0.15;
        totalTax = importDuty + supplementaryDuty + vat;

        fprintf(fp, "--- Motorcycle Duty Breakdown ---\n");
        fprintf(fp, "Import Duty (45%%): Tk %.2f\n", importDuty);
        fprintf(fp, "Supplementary Duty: Tk %.2f\n", supplementaryDuty);
        fprintf(fp, "VAT (15%%): Tk %.2f\n", vat);
        fprintf(fp, "Total Estimated Tax: Tk %.2f\n", totalTax);
    } else {
        printf("Invalid choice.\n");
        fprintf(fp, "Invalid selection.\n");
    }

    fclose(fp);
    printf("Duty calculation saved to file: %s\n", filename);
    return 0;
}



