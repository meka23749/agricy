async function loadLatest()
{
    try
    {
        const response =
            await fetch("/latest");

        const data =
            await response.json();

        document.getElementById(
            "temperature"
        ).textContent =
            data.temperature;

        document.getElementById(
            "humidity"
        ).textContent =
            data.humidity;

        document.getElementById(
            "lastUpdate"
        ).textContent =
            new Date().toLocaleString();
    }
    catch (error)
    {
        console.error(error);
    }
}

loadLatest();

setInterval(
    loadLatest,
    5000
);