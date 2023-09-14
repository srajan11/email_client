#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_EMAILS 100 // Maximum number of emails in the inbox
#define EMAILS_DIRECTORY "C:\\dlithe project"// Specify the email directory path

// Struct to store email details
typedef struct {
    char recipient[100];
    char subject[100];
    char message[500];
    int isDeleted; // 0 means not deleted, 1 means marked for deletion
} Email;

Email inbox[MAX_EMAILS];
int emailCount = 0; // Track the number of emails in the inbox

// Function to create the "emails" directory if it doesn't exist
void createEmailsDirectory() {
#ifdef _WIN32
    // Check if the directory already exists, and if not, create it
    struct stat st;
    if (stat(EMAILS_DIRECTORY, &st) == -1) {
        if (mkdir(EMAILS_DIRECTORY) != 0 && errno != EEXIST) {
            perror("Error creating email directory");
            exit(1);
        }
    }
#else
    // On non-Windows systems, use mkdir with permissions
    if (mkdir(EMAILS_DIRECTORY, 0777) != 0 && errno != EEXIST) {
        perror("Error creating email directory");
        exit(1);
    }
#endif
}

// Function to compose and save an email
void composeEmail() {
    if (emailCount >= MAX_EMAILS) {
        printf("Inbox is full. Cannot compose a new email.\n");
        return;
    }

    printf("Recipient (namenumber@gmail.com format): ");
    scanf("%s", inbox[emailCount].recipient);

    // Check if the email address is in the correct format
    if (strstr(inbox[emailCount].recipient, "@gmail.com") == NULL) {
        printf("Invalid email address format. Please use namenumber@gmail.com format.\n");
        return;
    }

    printf("Subject: ");
    scanf("%s", inbox[emailCount].subject);
    printf("Message: ");
    scanf(" %[^\n]s", inbox[emailCount].message);
    inbox[emailCount].isDeleted = 0; // Initialize as not deleted

    // Create the full path to the email file within the "emails" folder
    char filename[200]; // Adjust the size as needed
    snprintf(filename, sizeof(filename), "%s/email%d.txt", EMAILS_DIRECTORY, emailCount);

    // Create a new file and write email details
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error creating email file");
        return;
    }
    fprintf(fp, "Recipient: %s\n", inbox[emailCount].recipient);
    fprintf(fp, "Subject: %s\n", inbox[emailCount].subject);
    fprintf(fp, "Message: %s\n", inbox[emailCount].message);
    fclose(fp);

    printf("Email has been composed and saved in the inbox.\n");
    emailCount++;
}

// Function to list emails in the inbox
void listEmails() {
    printf("Inbox:\n");
    for (int i = 0; i < emailCount; i++) {
        if (!inbox[i].isDeleted) {
            printf("%d. From: %s   Subject: %s\n", i + 1, inbox[i].recipient, inbox[i].subject);
        }
    }
}

// Function to view a specific email
void viewEmail(int emailIndex) {
    if (emailIndex < 0 || emailIndex >= emailCount || inbox[emailIndex].isDeleted) {
        printf("Invalid email selection.\n");
        return;
    }

    printf("Viewing Email %d:\n", emailIndex + 1);
    printf("Recipient: %s\n", inbox[emailIndex].recipient);
    printf("Subject: %s\n", inbox[emailIndex].subject);
    printf("Message: %s\n", inbox[emailIndex].message);

    int option;
    printf("\nOptions:\n");
    printf("1. Reply to this email\n");
    printf("2. Forward this email\n");
    printf("3. Back to Inbox\n");
    printf("Enter your choice: ");
    scanf("%d", &option);

    switch (option) {
        case 1:
            replyToEmail(emailIndex);
            break;
        case 2:
            forwardEmail(emailIndex);
            break;
        case 3:
            // Do nothing, return to the inbox
            break;
        default:
            printf("Invalid option. Returning to the inbox.\n");
    }
}

// Function to reply to an email
void replyToEmail(int emailIndex) {
    if (emailCount >= MAX_EMAILS) {
        printf("Inbox is full. Cannot compose a reply.\n");
        return;
    }

    printf("Replying to Email %d:\n", emailIndex + 1);
    printf("Recipient (namenumber@gmail.com format): ");
    scanf("%s", inbox[emailCount].recipient);

    // Check if the email address is in the correct format
    if (strstr(inbox[emailCount].recipient, "@gmail.com") == NULL) {
        printf("Invalid email address format. Please use namenumber@gmail.com format.\n");
        return;
    }

    // Set the subject and message for the reply
    snprintf(inbox[emailCount].subject, sizeof(inbox[emailCount].subject), "Re: %s", inbox[emailIndex].subject);
    printf("Subject: %s\n", inbox[emailCount].subject);
    printf("Message: ");
    scanf(" %[^\n]s", inbox[emailCount].message);
    inbox[emailCount].isDeleted = 0; // Initialize as not deleted

    // Create the full path to the email file within the "emails" folder
    char filename[200]; // Adjust the size as needed
    snprintf(filename, sizeof(filename), "%s/email%d.txt", EMAILS_DIRECTORY, emailCount);

    // Create a new file and write email details
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error creating email file");
        return;
    }
    fprintf(fp, "Recipient: %s\n", inbox[emailCount].recipient);
    fprintf(fp, "Subject: %s\n", inbox[emailCount].subject);
    fprintf(fp, "Message: %s\n", inbox[emailCount].message);
    fclose(fp);

    printf("Reply email has been composed and saved in the inbox.\n");
    emailCount++;
}

// Function to forward an email
void forwardEmail(int emailIndex) {
    if (emailCount >= MAX_EMAILS) {
        printf("Inbox is full. Cannot forward this email.\n");
        return;
    }

    printf("Forwarding Email %d:\n", emailIndex + 1);
    printf("Recipient (namenumber@gmail.com format): ");
    scanf("%s", inbox[emailCount].recipient);

    // Check if the email address is in the correct format
    if (strstr(inbox[emailCount].recipient, "@gmail.com") == NULL) {
        printf("Invalid email address format. Please use namenumber@gmail.com format.\n");
        return;
    }

    // Set the subject and message for the forwarded email
    snprintf(inbox[emailCount].subject, sizeof(inbox[emailCount].subject), "Fwd: %s", inbox[emailIndex].subject);
    printf("Subject: %s\n", inbox[emailCount].subject);
    printf("Message: ");
    scanf(" %[^\n]s", inbox[emailCount].message);
    inbox[emailCount].isDeleted = 0; // Initialize as not deleted

    // Create the full path to the email file within the "emails" folder
    char filename[200]; // Adjust the size as needed
    snprintf(filename, sizeof(filename), "%s/email%d.txt", EMAILS_DIRECTORY, emailCount);

    // Create a new file and write email details
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error creating email file");
        return;
    }
    fprintf(fp, "Recipient: %s\n", inbox[emailCount].recipient);
    fprintf(fp, "Subject: %s\n", inbox[emailCount].subject);
    fprintf(fp, "Message: %s\n", inbox[emailCount].message);
    fclose(fp);

    printf("Forwarded email has been composed and saved in the inbox.\n");
    emailCount++;
}

// Function to mark an email for deletion
void markEmailForDeletion() {
    int emailIndex;

    printf("Enter the number of the email you want to mark for deletion: ");
    scanf("%d", &emailIndex);

    if (emailIndex > 0 && emailIndex <= emailCount) {
        inbox[emailIndex - 1].isDeleted = 1;
        printf("Email marked for deletion.\n");
    } else {
        printf("Invalid email selection.\n");
    }
}

// Function to permanently delete marked emails
void deleteMarkedEmails() {
    int markedCount = 0;

    for (int i = 0; i < emailCount; i++) {
        if (inbox[i].isDeleted) {
            markedCount++;
        } else if (markedCount > 0) {
            // Shift remaining emails to fill the gaps
            inbox[i - markedCount] = inbox[i];
        }
    }

    emailCount -= markedCount;
    printf("%d email(s) marked for deletion have been permanently deleted.\n", markedCount);
}

int main() {
    createEmailsDirectory(); // Create the "emails" directory if it doesn't exist

    int choice;

    while (1) {
        printf("\nSimple Email Client Menu:\n");
        printf("1. Compose Email\n");
        printf("2. View Emails\n");
        printf("3. Mark Email for Deletion\n");
        printf("4. Delete Marked Emails\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                composeEmail();
                break;
            case 2:
                listEmails();
                break;
            case 3:
                markEmailForDeletion();
                break;
            case 4:
                deleteMarkedEmails();
                break;
            case 5:
                printf("Exiting the email client. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please select a valid option.\n");
        }
    }

    return 0;
}
