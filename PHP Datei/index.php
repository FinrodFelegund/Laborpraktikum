<html>


    <form action="index.php" method="POST">
        Password :
        <input type='password' name='password'>
        <br>
        repeat Password :
        <input type='password' name='passwordRepeat'>
        <br>
    
        

        <button name="btn" type="submit">
           Reset
        </button>
    </form>
</html>

<?php
    session_start();
    $password = $_POST['password'];
    $passwordRepeat = $_POST['passwordRepeat'];

    $url = $_SERVER["HTTP_REFERER"];
    $components = parse_url($url);
    parse_str($components['query'], $results);
    $email = $results['email'];


    if(!empty($email))
    {
        $_SESSION['email'] = $email;

    } else
    {
        $email = $_SESSION['email'];
    }


    if(empty($password) || empty($passwordRepeat))
    {
        echo 'Please fill out the form!';
        exit();
    }

    

    if($password != $passwordRepeat)
    {
        echo 'Passwords do not match. Please try again';
        exit();
    }

    $hasLower = false; 
    $hasUpper = false;
    $hasDigit = false;
    $hasSpecial = false;
    $iseightCharsLong = false;
 
    if (!ctype_lower($password)) 
    {
        $hasUpper = true;
    }

    if(!ctype_upper($password))
    {
        $hasLower = true;
    }

    if(preg_match('~[0-9]+~', $password))
    {
        $hasDigit = true;
    }

    if (preg_match('!/[\'^£$%&*()}{@#~?><>,|=_+¬-]/', $password))
    {
        $hasSpecial = true;
    }

    if(strlen($password) > 7)
    {
        $iseightCharsLong = true;
    }


    if($hasLower && $hasUpper && $hasDigit && $hasSpecial && $iseightCharsLong)
    {

        $conn = new mysqli('127.0.0.1', 'LPRApp', 'DaCiHa22', 'demoapp');
    //echo "Reached this point"
    if($conn->connect_error)
    {
        die('Connection Failed : ' .$conn->connect_error);
    } else 
    {   
 

        $stmt = $conn->prepare("update users set passwort=md5('$password') where email='$email'");
        
        $stmt->execute();

        if(mysqli_affected_rows($conn) == 0 )
        {
            die('<br>'. 'This is not a valid Email Adress! Try again');

        } else {
            echo "Update successful!";
        }
        $stmt->close();
        
    }

    $conn->close();

    } else 
    {
        echo 'Password has to contain at least one uppercase, one lowercase letter, a digit and a special Character and has to be at least 8 character long!';
        exit();
    }

?>



 
