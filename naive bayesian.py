from csv import reader

# Load a CSV file


def load_csv(filename):
    dataset = list()
    with open(filename, 'r') as file:
        csv_reader = reader(file)
        for row in csv_reader:
            if not row:
                continue
            dataset.append(row)
    return dataset


# Make a prediction with Naive Bayes
filename = 'data.csv'
dataset = load_csv(filename)

# Calculate Probabilty
# initialization var
count_birth_yes_mam = 0
count_birth_no_mam = 0
count_birth_yes_non = 0
count_birth_no_non = 0

count_fly_no_mam = 0
count_fly_yes_mam = 0
count_fly_yes_non = 0
count_fly_no_non = 0

count_water_no_mam = 0
count_water_some_mam = 0

count_water_yes_mam = 0
count_water_no_non = 0
count_water_yes_non = 0
count_water_some_non = 0

count_lags_no_mam = 0
count_lags_yes_mam = 0
count_lags_no_non = 0
count_lags_yes_non = 0
for k in range(20):
    # 算出Give Birth的Frequency Table
    if dataset[k][1] == 'yes' and dataset[k][5] == 'mammals':
        count_birth_yes_mam += 1
    if dataset[k][1] == 'no' and dataset[k][5] == 'mammals':
        count_birth_no_mam += 1
    if dataset[k][1] == 'yes' and dataset[k][5] == 'non-mammals':
        count_birth_yes_non += 1
    if dataset[k][1] == 'no' and dataset[k][5] == 'non-mammals':
        count_birth_yes_non += 1
    # 算出Can Fly的Frequency Table
    if dataset[k][2] == 'yes' and dataset[k][5] == 'mammals':
        count_fly_yes_mam += 1
    if dataset[k][2] == 'no' and dataset[k][5] == 'mammals':
        count_fly_no_mam += 1
    if dataset[k][2] == 'yes' and dataset[k][5] == 'non-mammals':
        count_fly_yes_non += 1
    if dataset[k][2] == 'no' and dataset[k][5] == 'non-mammals':
        count_fly_no_non += 1
    # 算出Live in Water的Frequency Table
    if dataset[k][3] == 'yes' and dataset[k][5] == 'mammals':
        count_water_yes_mam += 1
    if dataset[k][3] == 'no' and dataset[k][5] == 'mammals':
        count_water_no_mam += 1
    if dataset[k][3] == 'sometimes' and dataset[k][5] == 'mammals':
        count_water_some_mam += 1

    if dataset[k][3] == 'yes' and dataset[k][5] == 'non-mammals':
        count_water_yes_non += 1
    if dataset[k][3] == 'no' and dataset[k][5] == 'non-mammals':
        count_water_yes_non += 1
    if dataset[k][3] == 'sometimes' and dataset[k][5] == 'non-mammals':
        count_water_some_non += 1
    # 算出Have lags的Frequency Table
    if dataset[k][4] == 'yes' and dataset[k][5] == 'mammals':
        count_lags_yes_mam += 1
    if dataset[k][4] == 'no' and dataset[k][5] == 'mammals':
        count_lags_no_mam += 1
    if dataset[k][4] == 'yes' and dataset[k][5] == 'non-mammals':
        count_lags_yes_non += 1
    if dataset[k][4] == 'no' and dataset[k][5] == 'non-mammals':
        count_lags_yes_non += 1

# give birth prob table
prob_birth_yes_mam = count_birth_yes_mam/7
prob_birth_no_mam = 1-prob_birth_yes_mam
prob_birth_yes_non = count_birth_yes_non/13
prob_birth_no_non = 1-prob_birth_yes_non
# can fly prob Table
prob_fly_yes_mam = count_fly_yes_mam/7
prob_fly_no_mam = 1-prob_fly_yes_mam
prob_fly_yes_non = count_fly_yes_non/13
prob_fly_no_non = 1-prob_fly_yes_non

# Live in water pro table
prob_water_yes_mam = count_water_yes_mam/7
prob_water_no_mam = 1-prob_water_yes_mam
prob_water_some_mam = count_water_some_mam/7
prob_water_yes_non = count_water_yes_non/13
prob_water_no_non = 1-prob_water_yes_non
prob_water_some_non = count_water_some_non/13

# Have lags pro table
prob_lags_yes_mam = count_lags_yes_mam/7
prob_lags_no_mam = 1-prob_lags_yes_mam
prob_lags_yes_non = count_lags_yes_non/13
prob_lags_no_non = 1-prob_lags_yes_non


# give birth, can fly, not live in water, have lags
case_1 = {'yes', 'yes', 'no', 'yes'}
# 是mammal的機率 = 是birth且是mammal的機率 * can fly且是mammals的機率
# * not live in water且是mammals的機率 * have lags且是mammals的機率
case_1_mam_prob = prob_birth_yes_mam * prob_fly_yes_mam * \
    prob_water_no_mam * prob_lags_yes_mam

case_1_non_prob = prob_birth_yes_non * prob_fly_yes_non * \
    prob_water_no_non * prob_lags_yes_non

if case_1_mam_prob > case_1_non_prob:
    print("Case 1 is a mammal")
else:
    print("Case 2 is not a mammal")

# give birth, can not  fly, live in water, not have lags
case_2_mam_prob = {'yes', 'no', 'yes', 'no'}

case_2_mam_prob = prob_birth_yes_mam * prob_fly_no_mam * \
    prob_water_yes_mam * prob_lags_no_mam

case_2_non_prob = prob_birth_yes_non * prob_fly_no_non * \
    prob_water_yes_non * prob_lags_no_non

if case_2_mam_prob > case_2_non_prob:
    print("Case 2 is a mammal")
else:
    print("Case 2 is not a mammal")

# not give birth, can not fly,sometimes live in water, not have lags
case_3_mam_prob = {'no', 'no', 'sometimes', 'no'}
case_3_mam_prob = prob_birth_no_mam * prob_fly_no_mam * \
    prob_water_some_mam * prob_lags_no_mam

case_3_non_prob = prob_birth_no_non * prob_fly_no_non * \
    prob_water_some_non * prob_lags_no_non

if case_3_mam_prob > case_3_non_prob:
    print("Case 3 is a mammal")
else:
    print("Case 3 is not a mammal")
