CREATE TABLE Partners_product(
ID INT IDENTITY(1,1) PRIMARY KEY NOT NULL,
Products NVARCHAR(250) NOT NULL,
ID_partners INT NOT NULL,
Quantity INT NOT NULL,
Date_sale DATE NOT NULL,
FOREIGN KEY (ID_partners) REFERENCES Partners(ID_partners) ON DELETE CASCADE
)